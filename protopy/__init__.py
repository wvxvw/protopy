# -*- coding: utf-8 -*-

import ctypes
import pkg_resources
import atexit

from sys import platform
from protopy.wrapped import apr_cleanup


if platform == 'win32':
    apr_dll = pkg_resources.resource_filename(
        __name__,
        '../lib/libapr-1.dll',
    )
    ctypes.WinDLL(apr_dll)

atexit.register(apr_cleanup)
