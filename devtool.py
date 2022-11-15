import os
import sys
import argparse
from subprocess import Popen, PIPE, STDOUT


def main():
    # define default variables
    appLocation = os.getcwd()
    args = parse_arguments()
    process = start_subprocess(args)

    commands = prepare_intel(args) if args.compiler == 'intel' else ""

    match args.command:
        case 'build':
            commands = commands + command_build(args)
        case 'test':
            commands = commands + command_test(args)
        case 'app':
            commands = commands + command_app(args)

    execute_commands(process, commands)


def parse_arguments():
    # define cli args
    parser = argparse.ArgumentParser(prog="PROG")
    subparsers = parser.add_subparsers(help='subcommand help', dest="command")

    # the build subcommand
    build_parser = subparsers.add_parser('build', help="build subcommand help")
    build_parser.add_argument(
        "-c", "--compiler", default='gnu', help="Choose compiler, could be 'intel', 'gnu'. Default gnu")
    build_parser.add_argument(
        "-b", "--build", default='Debug', help="Choose build type, could be 'Debug', 'Release'. Default Debug"
    )
    build_parser.add_argument(
        '-t', "--target", default='all', help="Choose build target, default 'all'"
    )
    build_parser.add_argument(
        '-s', "--system", default=sys.platform, help="Choose build system, default to current system"
    )
    build_parser.add_argument(
        '-d', "--cmakedir", default="", help="Choose build cmake bin directory, default to ''"
    )

    # the test subcommand
    test_parser = subparsers.add_parser('test', help="test subcommand help")
    test_parser.add_argument(
        "-c", "--compiler", default='gnu', help="Choose compiler, could be 'intel', 'gnu'. Default gnu")
    test_parser.add_argument(
        "-b", "--build", default='Release', help="Choose test type, could be 'Debug', 'Release'. Default Release"
    )
    test_parser.add_argument(
        '-t', "--target", default='all', help="Choose test target, default 'all'"
    )
    test_parser.add_argument(
        '-s', "--system", default=sys.platform, help="Choose test system, default to current system"
    )
    test_parser.add_argument(
        '-d', "--cmakedir", default="NotUsed", help="Choose test cmake bin directory, default to 'NotUsed'"
    )

    # parse the args
    args = parser.parse_args(sys.argv[1:])
    print(args)
    return args


def start_subprocess(args):
    match args.system:
        case "linux":
            process = Popen('bash', shell=False, universal_newlines=True,
                            stdin=PIPE, stdout=PIPE, stderr=STDOUT)
        case "darwin":
            process = Popen('zsh', shell=False, universal_newlines=True,
                            stdin=PIPE, stdout=PIPE, stderr=STDOUT)
        case "win32":
            if args.compiler == 'intel':
                process = Popen('cmd.exe', shell=False, universal_newlines=True,
                                stdin=PIPE, stdout=PIPE, stderr=STDOUT)
            else:
                process = Popen('mingw64.exe', shell=False, universal_newlines=True,
                                stdin=PIPE, stdout=PIPE, stderr=STDOUT)
    return process


def prepare_intel(args):
    match args.system:
        case "linux":
            source_intel = """
            source /opt/intel/oneapi/setvars.sh
            """
            print("inside linux")
        case "darwin":
            source_intel = """
            source /opt/intel/oneapi/setvars.sh
            export CC=icc
            export CXX=icpc
            """
        case "win32":
            source_intel = """
            C:/Program Files(x86)/Intel/oneAPI/setvars.bat
            """
    return source_intel


def cmake_cmd(args, bin):
    cmakedir = ''
    if args.cmakedir == "NotUsed":
        if args.system == "win32" and args.system == "intel":
            cmakedir = "C:\\PROGRAM FILES (X86)\\MICROSOFT VISUAL STUDIO\\2019\\COMMUNITY\\COMMON7\\IDE\\COMMONEXTENSIONS\\MICROSOFT\\CMAKE\\CMake\\bin"
        else:
            cmakedir = ''
    else:
        cmakedir = args.cmakedir

    return os.path.join(cmakedir, bin)


def command_test(args):
    ctest = cmake_cmd(args, 'ctest')
    return command_build(args)+f"""
    {ctest} --preset="{args.system}-{args.compiler}-{args.build}"
    """


def command_build(args):
    cmake = cmake_cmd(args, 'cmake')
    return f"""
    {cmake} --preset="{args.system}-{args.compiler}-{args.build}" -S "."
    {cmake} --build --preset="{args.system}-{args.compiler}-{args.build}" --target {args.target}
    """


def command_app(app_location, args):
    return f"""
    cp CMakePresets.json {app_location}/
    cp -r 
    cd {app_location}
    """+command_build(args)+f"""
    rm CMakePresets.json
    cd -
    """

# execute the command


def execute_commands(process, commands):
    for cmd in commands:
        process.stdin.write(cmd)
    process.stdin.close()

    for line in process.stdout:
        print(line, end="")


if __name__ == "__main__":
    main()
