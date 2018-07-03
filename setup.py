#!/usr/bin/env python3.6
# -*- coding: utf-8 -*-

import os
from sys import platform
# from distutils.core import setup, Extension
from setuptools import setup, Extension

# './include/apr/unix/.libs/libapr-1.so'

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
