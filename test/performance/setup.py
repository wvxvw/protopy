#!/usr/bin/env python3.6
# -*- coding: utf-8 -*-

from setuptools import setup


setup(
    packages=['protopy_performance'],
    name='protopy',
    version='0.0.3',
    description='Performance testing for protopy',
    author='Oleg Sivokon',
    author_email='olegs@traiana.com',
    url='TBD',
    license='PROPRIETARY',
    scripts=['bin/protopy-performance'],
    install_requires=[
        'protopy >= 0.0.3',
        'mnemonicode >= 1.4.4',
    ],
)
