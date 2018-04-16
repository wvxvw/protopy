def __bootstrap__():
    global __bootstrap__, __loader__, __file__
    import sys, pkg_resources, imp, atexit
    __file__ = pkg_resources.resource_filename(
        __name__,
        'wrapped.cpython-36m-x86_64-linux-gnu.so',
    )
    __loader__ = None
    del __bootstrap__, __loader__
    imp.load_dynamic(__name__, __file__)
    from protopy.wrapped import apr_cleanup
    atexit.register(apr_cleanup)


__bootstrap__()
