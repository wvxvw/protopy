#!/usr/bin/env python3.6
# -*- coding: utf-8 -*-

# from distutils.core import setup, Extension
from setuptools import setup, Extension

# './include/apr/unix/.libs/libapr-1.so'

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
    package_data={
        'protopy/test/resources': ['test/resources/*.proto'],
    },
    # data_files=[('test/resources', 'test/resources/*.proto')],
    ext_package='protopy',
    ext_modules=[
        Extension(
            'wrapped',
            [
                'protopy/lib/binparser.c',
                'protopy/lib/defparser.c',
                'protopy/lib/protopy.lex.c',
                'protopy/lib/protopy.tab.c',
                'protopy/lib/pyhelpers.c',
                'protopy/lib/list.c',
                'protopy/wrapper.c',
            ],
            include_dirs=['/usr/include/apr-1.0'],
            libraries=['apr-1'],
            # include_dirs=['./include/apr/unix/include']
            # extra_link_args=['-Wl,-rpath,{}'.format(LIB_DIR)]
        )
    ],
    install_requires=[
        'pytest >= 3.4.2',
    ],
)
