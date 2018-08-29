# -*- coding: utf-8 -*-

import pkg_resources

from protopy.parser import BinParser, simple_enum

from support import generate_proto_binary


def test_gen_load_file():
    roots, test_proto, content = generate_proto_binary(
        'test.proto',
        b'test: 123\ntest_whatever: "123456"',
    )
    print('generated proto message: {}'.format(content))

    # for _ in range(100):
    #     result = BinParser(roots).parse(test_proto, 'Test', content)

    result = BinParser(roots).parse(test_proto, 'Test', content)

    print('result: {}'.format(result))
    assert result.test == 123


def try_gc():
    import gc
    gc.set_debug(gc.DEBUG_LEAK)
    gc.collect()

    for g in gc.garbage:
        print('gc.garbage: {}'.format(g))


def test_inner_message():
    roots, test_proto, content = generate_proto_binary(
        'test_embedded.proto',
        b'''test: 123
            first_inner: {test_fixed: 4567}
            second_inner: {test_long: 135, test_short: 689}
        ''',
    )
    print('generated proto message: {}'.format(content))

    result = BinParser(roots).parse(test_proto, 'Test', content)

    print('result: {}'.format(result))
    assert result.test == 123


def test_enum():
    roots, test_proto, content = generate_proto_binary(
        'test_enum.proto',
        b'''test_1: 33
            test_2: 2
            test_3: 0
            test_4: 5
        ''',
    )
    print('generated proto message: {}'.format(content))

    result = BinParser(roots).parse(test_proto, 'Test', content)

    print('result: {}'.format(result))
    TestEnum = type(result.test_1)
    assert result.test_1 == TestEnum['TEST_MEMBER_3']


def test_simple_enum():
    roots, test_proto, content = generate_proto_binary(
        'test_enum.proto',
        b'''test_1: 33
            test_2: 2
            test_3: 0
            test_4: 5
        ''',
    )
    print('generated proto message: {}'.format(content))

    parser = BinParser(roots)
    parser.def_parser.enum_ctor = simple_enum
    result = parser.parse(test_proto, 'Test', content)

    print('result: {}'.format(result))
    assert result.test_1 == 'TestEnum.TEST_MEMBER_3'


def test_repeated():
    roots, test_proto, content = generate_proto_binary(
        'test_repeated.proto',
        b'''simple_repeats: {
            some_ints: [1, -1, 2, -2, 3, -3, 4, -4, 5, -5]
        }
        multiple_repeats: [{
            some_fixed: [1, -1, 2, -2, 3, -3]
            some_strings: ["foo", "bar", "\x01weird\x10"]
        },
        {
            some_fixed: [4, -4, 5, -5, 6, -6]
        }]
        multiple_oneof: {
            some_fixed: [123456789, 987654321]
            some_strings: [""]
        }
        ''',
    )
    print('generated proto message: {}'.format(content))

    result = BinParser(roots).parse(test_proto, 'Test', content)

    print('result: {}'.format(result))
    assert result.multiple_oneof.some_fixed == [123456789, 987654321]


def test_fixed64():
    roots, test_proto, content = generate_proto_binary(
        'test_fixed64.proto',
        b'''sfixed_fixed: [{
            key: -1
            value: 123
        },
        {
            key: -1000
            value: 1000
        }]
        simple_fixed: 1010101010
        simple_sfixed: -1010101010
        ''',
    )
    print('generated proto message: {}'.format(content))

    result = BinParser(roots).parse(test_proto, 'Test', content)

    print('result: {}'.format(result))
    assert result.simple_fixed == 1010101010
    # try_gc()


def test_1_fixed64():
    roots, test_proto, content = generate_proto_binary(
        'test_fixed64.proto',
        b'''some_fixed: [1234, 12, 9876, 0]
        some_sfixed: [-9876, 9876, 0, -1, 1]
        sfixed_fixed: [{
            key: -1
            value: 1
        },
        {
            key: -1000
            value: 1000
        }]
        simple_fixed: 1010101010
        simple_sfixed: -1010101010
        ''',
    )
    print('generated proto message: {}'.format(content))

    result = BinParser(roots).parse(test_proto, 'Test', content)

    print('result: {}'.format(result))
    assert result.some_fixed[1] == 12


def test_map():
    roots, test_proto, content = generate_proto_binary(
        'test_map.proto',
        b'''simple_map: [{
                key: "foo"
                value: 1
            },
            {
                key: "bar"
                value: 2
            },
            {
                key: "baz"
                value: 3
            }]
            inner_map: [{
                key: 1
                value: {
                    sint_uint: [{
                        key: 1
                        value: 1
                    },
                    {
                        key: 2,
                        value: 2
                    },
                    {
                        key: 3
                        value: 3
                    }]
                }
            },
            {
                key: 2
                value: {
                    sint_uint: [{
                        key: -1
                        value: 1
                    },
                    {
                        key: -2,
                        value: 2
                    },
                    {
                        key: -3
                        value: 3
                    }]
                }
            },
            {
                key: 3
                value: {
                    sint_uint: [{
                        key: -1
                        value: 2
                    },
                    {
                        key: -2,
                        value: 4
                    },
                    {
                        key: -3
                        value: 6
                    }]
                }
            }]
        inner: {
            sint_uint: [{
                key: 1
                value: 1
            },
            {
                key: 2,
                value: 2
            },
            {
                key: 3
                value: 3
            }]
        }
        inner_inner: {
            bytes_inner_map: [{
                key: 123
                value: {
                    sint_uint: [{
                        key: -1
                        value: 4
                    },
                    {
                        key: -2,
                        value: 8
                    },
                    {
                        key: -3
                        value: 12
                    }]
                }
            },
            {
                key: 456
                value: {
                    sint_uint: [{
                        key: -1
                        value: 8
                    },
                    {
                        key: -2,
                        value: 16
                    },
                    {
                        key: -3
                        value: 24
                    }]
                }
            },
            {
                key: 789
                value: {
                    sint_uint: [{
                        key: 1
                        value: 16
                    },
                    {
                        key: 2,
                        value: 32
                    },
                    {
                        key: 3
                        value: 48
                    }]
                }
            }]
        }
        inner_inner_inner: {
            string_inner_inner_map: [{
                key: "foo"
                value: {
                    bytes_inner_map: [{
                        key: 12
                        value: {
                            sint_uint: [{
                                key: 1
                                value: 16
                            },
                            {
                                key: 2,
                                value: 32
                            },
                            {
                                key: 3
                                value: 48
                            }]
                        }
                    },
                    {
                        key: 34
                        value: {
                            sint_uint: [{
                                key: 1
                                value: 16
                            },
                            {
                                key: 2,
                                value: 32
                            },
                            {
                                key: 3
                                value: 48
                            }]
                        }
                    }]
                },
            },
            {
                key: "bar"
                value: {
                    bytes_inner_map: [{
                        key: 56
                        value: {
                            sint_uint: [{
                                key: 1
                                value: 16
                            },
                            {
                                key: 2,
                                value: 32
                            },
                            {
                                key: 3
                                value: 48
                            }]
                        }
                    },
                    {
                        key: 78
                        value: {
                            sint_uint: [{
                                key: 1
                                value: 16
                            },
                            {
                                key: 2,
                                value: 32
                            },
                            {
                                key: 3
                                value: 48
                            }]
                        }
                    }]
                }
            }]
        }
        ''',
    )
    print('generated proto message: {}'.format(content))

    result = BinParser(roots).parse(test_proto, 'Test', content)

    print('result: {}'.format(result))
    assert result.inner_inner_inner.string_inner_inner_map['bar'] \
                                   .bytes_inner_map[78].sint_uint[2] == 32


def test_invalid_source_path():
    roots = [pkg_resources.resource_filename(
        __name__,
        './resources',
    )]
    try:
        BinParser(roots).parse(None, 'Test', 'does not matter')
    except FileNotFoundError:
        pass
    roots, test_proto, content = generate_proto_binary(
        'test.proto',
        b'test: 123\ntest_whatever: "123456"',
    )
    print('generated proto message: {}'.format(content))

    parser = BinParser(roots)
    parser.parse(test_proto, 'Test', content)
    try:
        parser.parse(None, 'Test', content)
    except FileNotFoundError:
        pass


def test_replace_ctor():
    roots, test_proto, content = generate_proto_binary(
        'test_wrapped.proto',
        b'test: "abcdefg"',
        'Wrapped'
    )
    print('generated proto message: {}'.format(content))
    roots, test_proto, content = generate_proto_binary(
        'test_wrapped.proto',
        b'''
        wrapped_type: "Wrapped"
        wrapped_payload: "%s"
        ''' % repr(content).encode('utf-8')[2:-1],
        'Wrapper'
    )

    parser = BinParser(roots)

    class ModifiedWrapper:

        original = None

        def __init__(self, *args):
            print('modified init')
            raw = ModifiedWrapper.original(*args)
            self.payload = parser.parse(
                test_proto,
                raw.wrapped_type,
                raw.wrapped_payload,
            )
            self.wrapped_type = raw.wrapped_type
            print('modified init finished')

    result = parser.parse(test_proto, 'Wrapper', content)
    print('result before: {}'.format(result))
    assert result.wrapped_type == 'Wrapped'

    ModifiedWrapper.original = parser.def_parser.find_definition(b'Wrapper')
    print('ModifiedWrapper.original: {}'.format(ModifiedWrapper.original))
    parser.def_parser.update_definition(b'Wrapper', ModifiedWrapper)

    result = parser.parse(test_proto, 'Wrapper', content)
    for k, v in parser.def_parser.definitions():
        print('def: {} => {}'.format(k, v))
    print('result after: {}'.format(result))
    assert result.wrapped_type == 'Wrapped'
    assert result.payload.test == 'abcdefg'


def test_replace_func():
    roots, test_proto, content = generate_proto_binary(
        'test_wrapped.proto',
        b'test: "abcdefg"',
        'Wrapped'
    )
    print('generated proto message: {}'.format(content))
    roots, test_proto, content = generate_proto_binary(
        'test_wrapped.proto',
        b'''
        wrapped_type: "Wrapped"
        wrapped_payload: "%s"
        ''' % repr(content).encode('utf-8')[2:-1],
        'Wrapper'
    )

    parser = BinParser(roots)

    result = parser.parse(test_proto, 'Wrapper', content)
    print('result before: {}'.format(result))
    assert result.wrapped_type == 'Wrapped'

    original = parser.def_parser.find_definition(b'Wrapper')
    print('original: {}'.format(original))

    def replacement(*args):
        print('replacement')
        return {'replaced': original(*args)}
    parser.def_parser.update_definition(b'Wrapper', replacement)

    result = parser.parse(test_proto, 'Wrapper', content)
    print('result after: {}'.format(result))
    assert result['replaced'].wrapped_type == 'Wrapped'


def test_replace_method():
    roots, test_proto, content = generate_proto_binary(
        'test_wrapped.proto',
        b'test: "abcdefg"',
        'Wrapped'
    )
    print('generated proto message: {}'.format(content))
    roots, test_proto, content = generate_proto_binary(
        'test_wrapped.proto',
        b'''
        wrapped_type: "Wrapped"
        wrapped_payload: "%s"
        ''' % repr(content).encode('utf-8')[2:-1],
        'Wrapper'
    )

    parser = BinParser(roots)

    result = parser.parse(test_proto, 'Wrapper', content)
    print('result before: {}'.format(result))
    assert result.wrapped_type == 'Wrapped'

    original = parser.def_parser.find_definition(b'Wrapper')
    print('original: {}'.format(original))

    class Replacement:

        def __init__(self, original):
            self.original = original

        def replacement(self, *args):
            print('replacement')
            return {'replaced': self.original(*args)}

    rp = Replacement(original)
    parser.def_parser.update_definition(b'Wrapper', rp.replacement)

    result = parser.parse(test_proto, 'Wrapper', content)
    print('result after: {}'.format(result))
    print('new def: {}'.format(parser.def_parser.find_definition('Wrapper')))
    assert result['replaced'].wrapped_type == 'Wrapped'


def test_dotted():
    roots, test_proto, content = generate_proto_binary(
        'test_dotted_type.proto',
        b'''dotted_repeats: [{
            int_field: 100,
        },
        {
            int_field: 1000,
        }]
        inner_dotted: {
            string_field: "abcde"
        }
        imported_dotted: {
            a_string: "xyz"
            an_int: 54321
        }
        one_of_inner_dotted: {
            string_field: "Lorem ipsum"
        }
        ''',
    )
    print('generated proto message: {}'.format(content))

    parser = BinParser(roots)
    parser.def_parser.parse(test_proto)

    for f, p in parser.def_parser.definitions():
        print('{} => {}'.format(f, p))

    print('all definitions parsed')
    result = parser.parse(test_proto, 'Test', content)

    print('result: {}'.format(result))
    assert result.inner_dotted.string_field == "abcde"


def test_keywords():
    roots, test_proto, content = generate_proto_binary(
        'test_keywords.proto',
        b'''
        False: "1"
        None: "2"
        True: "3"
        and: "4"
        as: "5"
        assert: "6"
        break: "7"
        class: "8"
        continue: "9"
        def: "10"
        del: "11"
        elif: "12"
        else: "13"
        except: "14"
        finally: "15"
        for: "16"
        from: "17"
        global: "18"
        if: "19"
        import: "20"
        in: "21"
        is: "22"
        lambda: "23"
        nonlocal: "24"
        not: "25"
        or: "26"
        pass: "27"
        raise: "28"
        return: "29"
        try: "30"
        while: "31"
        with: "32"
        yield: "33"
        ''',
    )
    print('generated proto message: {}'.format(content))

    result = BinParser(roots).parse(test_proto, 'Test', content)

    print('result: {}'.format(result))
    assert result.pb_yield == "33"
