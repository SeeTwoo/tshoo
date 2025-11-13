#!/usr/bin/env python3
import subprocess, difflib, sys, pathlib

# Each line in commands.t is a test command
COMMAND_FILE = pathlib.Path("commands.t")

def run(shell_path, cmd, ref=False):
    exe = ["bash", "--posix", "-c", cmd] if ref else [shell_path, "-c", cmd]
    return subprocess.run(exe, capture_output=True, text=True)

def compare(name, ref, sut):
    if (ref.stdout, ref.stderr, ref.returncode) != (sut.stdout, sut.stderr, sut.returncode):
        print(f"\n{name}: \x1b[31mMismatch\x1b[0m")
        diff = difflib.unified_diff(
            ref.stdout.splitlines(keepends=True),
            sut.stdout.splitlines(keepends=True),
            fromfile="bash",
            tofile="your shell",
        )
        sys.stdout.writelines(diff)
        print(f"bash return {ref.returncode}, your shell returned {sut.returncode}")
        return False
    print(f"{name}: \x1b[32mOK\x1b[0m")
    return True

def main():
    if len(sys.argv) != 2:
        print("usage: tester.py /path/to/your_shell", file=sys.stderr)
        sys.exit(2)

    shell_path = sys.argv[1]
    if not pathlib.Path(COMMAND_FILE).exists():
        print(f"{COMMAND_FILE} not found", file=sys.stderr)
        sys.exit(2)

    ok = True
    with open(COMMAND_FILE) as f:
        for i, line in enumerate(f, 1):
            cmd = line.strip()
            if not cmd or cmd.startswith("#"):
                continue
            ref = run(shell_path, cmd, ref=True)
            sut = run(shell_path, cmd)
            if not compare(f"line {i}: {cmd}", ref, sut):
                ok = False
    sys.exit(0 if ok else 1)

if __name__ == "__main__":
    main()
