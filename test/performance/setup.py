#!/usr/bin/env python3.6
# -*- coding: utf-8 -*-

from setuptools import setup


setup(
    packages=['protopy_performance'],
    name='protopy-performance',
    version='0.0.3',
    description='Performance testing for protopy',
    author='Oleg Sivokon',
    author_email='olegs@traiana.com',
    url='TBD',
    entry_points={
        'console_scripts': [
            'perfgen = protopy_performance.cli:perfgen',
            'perfrunner = protopy_performance.cli:perfrunner',
        ]
    },
    license='PROPRIETARY',
    dependency_links=[
        'https://github.com/wvxvw-traiana/protopy/releases/download/'
        'v0.0.3/protopy-0.0.3-cp36-cp36m-linux_x86_64.whl'
    ],
    install_requires=[
        'protopy >= 0.0.3',
        'mnemonicode >= 1.4.4',
    ],
)
