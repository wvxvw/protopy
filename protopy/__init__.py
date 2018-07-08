# -*- coding: utf-8 -*-

import ctypes
import pkg_resources
import atexit

from sys import platform


if platform == 'win32':
    apr_dll = pkg_resources.resource_filename(
        __name__,
        '../../libapr-1.dll',
    )
    ctypes.WinDLL(apr_dll)

from protopy.wrapped import apr_cleanup  # noqa: E402

atexit.register(apr_cleanup)
