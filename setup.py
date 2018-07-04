#!/usr/bin/env python3.6
# -*- coding: utf-8 -*-

import os
import logging
from sys import platform
from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
from subprocess import Popen, CalledProcessError, check_output


project_dir = os.path.dirname(os.path.realpath(__file__))


class BuildWithYacc(build_ext):

    yacc = 'yacc'
    lex = 'lex'

    def has_yacc_and_lex(self):
        finder = None
        if platform == 'win32':
            finder = 'where'
        else:
            finder = 'which'

        result = 1

        for p in (self.yacc, self.lex):
            out = check_output([finder, p]).strip()
            if not out:
                logging.warning('Did not find {}'.format(self.yacc))
            result &= bool(out)
        return result

    def run_proc_with_setup(self, cwd, env, args):
        try:
            print(' '.join(args))
            proc = Popen(args, env=env, cwd=cwd)
            stdout, stderr = proc.communicate()
            if stdout:
                print(stdout)
            if proc.returncode != 0:
                raise Exception(str(stderr))
        except CalledProcessError as e:
            print(e.stderr)
            raise

    def generate_parser(self):
        yacc_args = [
            self.yacc,
            '-t',
            '-vd',
            'protopy.y',
            '-o',
            'protopy.tab.c',
        ]
        lex_args = [
            self.lex,
            '-d',
            '--header-file=protopy.lex.h',
            '-o',
            'protopy.lex.c',
            'protopy.l'
        ]
        cwd = os.path.join(project_dir, 'protopy', 'lib')
        env = dict(os.environ)

        self.run_proc_with_setup(cwd, env, yacc_args)
        self.run_proc_with_setup(cwd, env, lex_args)

    def run(self):
        if self.has_yacc_and_lex():
            self.generate_parser()
        else:
            logging.warning(
                'Did not find yacc and or lex, will use precompiled parser',
            )
        super().run()


apr_lib_candidates = (
    '/usr/include/apr-1.0',
    '/usr/include/apr-1',
)

apr_lib = None
extra_compile_args = None
package_data = {}
library_dirs = None
data_files = None

if platform == 'win32':
    apr_lib = os.path.join(
        os.path.dirname(os.path.realpath(__file__)),
        'include/apr',
    )
    package_data = {
        'lib': ['lib/libapr-1.dll'],
    }
    library_dirs = ['lib/apr']
    data_files = [('lib', ['lib/libapr-1.dll'])]
else:
    extra_compile_args = ['-std=c11']
for lib in apr_lib_candidates:
    if os.path.isdir(lib):
        apr_lib = lib
        break

if not apr_lib:
    raise Exception('Cannot find Apache Portable Runtime headers')

setup(
    cmdclass={'build_ext': BuildWithYacc},
    packages=['protopy'],
    name='protopy',
    version='0.0.1',
    description='Parser for Google Protobuf',
    author='Oleg Sivokon',
    author_email='olegs@traiana.com',
    url='TBD',
    license='PROPRIETARY',
    scripts=['bin/protopy'],
    package_data=package_data,
    data_files=data_files,
    ext_package='protopy',
    ext_modules=[
        Extension(
            'wrapped',
            [
                'protopy/lib/descriptors.c',
                'protopy/lib/binparser.c',
                'protopy/lib/serializer.c',
                'protopy/lib/defparser.c',
                'protopy/lib/protopy.lex.c',
                'protopy/lib/protopy.tab.c',
                'protopy/lib/pyhelpers.c',
                'protopy/lib/list.c',
                'protopy/wrapper.c',
            ],
            include_dirs=[apr_lib],
            library_dirs=library_dirs,
            libraries=['apr-1'],
            py_limited_api=False,
            extra_compile_args=extra_compile_args,
            # include_dirs=['./include/apr/unix/include']
            # extra_link_args=['-Wl,-rpath,{}'.format(LIB_DIR)]
        )
    ],
    install_requires=[
        'pytest >= 3.4.2',
    ],
)
