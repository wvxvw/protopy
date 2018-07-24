#!/usr/bin/env python3.6
# -*- coding: utf-8 -*-

from setuptools import setup

setup(
    packages=['tictactoe'],
    name='protopy-tictactoe',
    version='0.0.1',
    description='Example project for protopy',
    author='Oleg Sivokon',
    author_email='olegs@traiana.com',
    url='TBD',
    license='PROPRIETARY',
    scripts=['bin/tictactoe-server', 'bin/tictactoe-client'],
    package_data={'tictactoe/etc': ['etc/tictactoe.proto']},
    data_files=[('tictactoe/etc', ['etc/tictactoe.proto'])],
    dependency_links=[
        'https://github.com/wvxvw-traiana/protopy/releases/download/'
        'v0.0.3/protopy-0.0.3-cp36-cp36m-linux_x86_64.whl',
        'https://github.com/wvxvw-traiana/protopy/releases/download/'
        'v0.0.3/protopy-0.0.3-cp36-cp36m-win_amd64.whl'
    ],
    install_requires=[
        'protopy >= 0.0.3',
    ],
)
