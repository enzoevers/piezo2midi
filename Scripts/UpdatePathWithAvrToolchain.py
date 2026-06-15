import argparse
import os
from pathlib import Path
import sys
import utils


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Add AVR toolchain paths to PATH for GitHub Actions or emit commands for a calling shell."
    )
    parser.add_argument("--github-ci", action="store_true", help="Write paths to GITHUB_PATH")
    parser.add_argument(
        "--shell",
        choices=["powershell", "bash", "cmd"],
        default="powershell",
        help="Shell syntax to emit when using --print-command (default: powershell)",
    )
    parser.add_argument(
        "--print-command",
        action="store_true",
        default=False,
        help="Print shell command to add toolchain paths to PATH in the calling shell",
    )
    parser.add_argument("--toolchain-path", default=str((utils.get_project_root() / "avr-toolchain").resolve()))
    return parser.parse_args()


def build_shell_command(entries: list[str], shell: str) -> str:
    if shell == "powershell":
        value = ";".join(entries).replace("'", "''")
        return f"$env:PATH += ';{value}'"

    if shell == "bash":
        value = ":".join(entries)
        return f'export PATH="$PATH:{value}"'

    # cmd.exe
    value = ";".join(entries)
    return f'set "PATH=%PATH%;{value}"'


def windows_abs_path_to_unix_style(path: str) -> str:
    # Convert "C:\path\to\dir" to "/c/path/to/dir"
    drive, tail = os.path.splitdrive(path)
    drive_letter = drive[:-1].lower()  # Remove the colon and convert to lowercase
    unix_path = tail.replace("\\", "/")  # Replace backslashes with forward slashes
    return f"/{drive_letter}{unix_path}"


def main() -> int:
    args = parse_args()
    toolchain_path = Path(args.toolchain_path).resolve()

    entries = [
        str(toolchain_path / "avr8-gnu-toolchain" / "bin"),
        str(toolchain_path / "avr8-gnu-toolchain" / "avr" / "bin"),
        str(toolchain_path / "gnuwin32" / "bin"),
    ]

    if args.shell == "bash":
        entries = [windows_abs_path_to_unix_style(entry) for entry in entries]

    if args.github_ci:
        github_path = os.environ.get("GITHUB_PATH")
        if not github_path:
            raise RuntimeError("GITHUB_PATH is not set")

        with open(github_path, "a", encoding="utf-8") as file:
            for entry in entries:
                file.write(entry + "\n")
        return 0

    if args.print_command:
        print(build_shell_command(entries, args.shell))
        return 0

    print("Cannot modify parent shell PATH directly from Python.")
    print("Use one of these commands in your shell:")
    print(
        '  PowerShell: Invoke-Expression "$(python ./Scripts/UpdatePathWithAvrToolchain.py --print-command --shell powershell)"'
    )
    print('  Bash: eval "$(python ./Scripts/UpdatePathWithAvrToolchain.py --print-command --shell bash)"')
    print(
        "  CMD: for /f \"delims=\" %i in ('python .\\Scripts\\UpdatePathWithAvrToolchain.py --print-command --shell cmd') do %i"
    )
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except Exception as exc:  # noqa: BLE001
        print(f"Error: {exc}", file=sys.stderr)
        raise SystemExit(1)
