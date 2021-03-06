from argparse import ArgumentParser
from functools import reduce

from protopy_performance.generator import ProtoGenerator
from protopy_performance.runner import Runner


def perfgen():
    parser = ArgumentParser()
    parser.add_argument(
        '-s', '--seed',
        type=int,
        help='Seed to use when generating Proto file',
        required=True,
    )
    parser.add_argument(
        '-o', '--output',
        type=str,
        help='Protobuf IDL file to save output to',
        required=True,
    )
    parser.add_argument(
        '-b', '--binary-mask',
        type=str,
        help='''
    Protobuf binary file mask to save output to.

    Use '%%d' in the mask where you want the the number assigned to
    this file during generation to appear.  See also
    `--num-binaries'.
    ''',
        required=False,
        default='proto-%d.bin',
    )
    parser.add_argument(
        '-n', '--num-binaries',
        type=int,
        help='How many binary outputs to produce.',
        required=False,
        default=0,
    )
    parser.add_argument(
        '-x', '--binary-seeds',
        type=int,
        help='Seeds for binary proto files.',
        required=False,
        action='append',
        nargs='+',
    )

    args = parser.parse_args()
    generator = ProtoGenerator(args.seed)

    with open(args.output, 'w') as o:
        for line in generator.render().split('\n'):
            if line.strip():
                print(line, file=o)

    if args.num_binaries:
        generator.bin(
            args.output,
            args.binary_seeds,
            args.binary_mask,
            args.num_binaries,
        )


def perfrunner():
    parser = ArgumentParser()
    parser.add_argument(
        '-n', '--times',
        type=int,
        help='How many times to run the test.',
        default=100,
    )
    parser.add_argument(
        '-f', '--proto-file',
        type=str,
        help='The file containing the definition generated by perfgen.',
        required=True,
    )
    parser.add_argument(
        '-p', '--python-proto-file',
        type=str,
        help='The file containing the Python definition generated by protoc.',
        required=True,
    )
    parser.add_argument(
        '-j', '--nthreads',
        type=int,
        help='How many threads to use when running the test.',
        default=1,
    )
    parser.add_argument(
        '-e', '--optimize-enum-ctor',
        action='store_true',
        help='Should we use simplified enum constructor.',
    )
    parser.add_argument(
        '-m', '--optimize-message-ctor',
        action='store_true',
        help='Should we use simplified message constructor.',
    )
    parser.add_argument(
        '-b', '--binary-files',
        type=str,
        help='Binaries generated by perfgen.',
        action='append',
        required=True,
        nargs='+',
    )

    args = parser.parse_args()
    if not isinstance(args.binary_files[0], str):
        # We were called like this: runner -b foo*.bin -b bar.bin
        args.binary_files = reduce(lambda a, b: a + b, args.binary_files, [])

    runner = Runner(
        args.proto_file,
        args.python_proto_file,
        args.binary_files,
        args.times,
        args.nthreads,
        args.optimize_enum_ctor,
        args.optimize_message_ctor,
    )
    runner.run_protopy()
    runner.run_protobuf()
