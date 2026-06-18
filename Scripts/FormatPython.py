#!/usr/bin/env python3
from __future__ import annotations

import argparse
import shutil
import subprocess
import sys
from pathlib import Path


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Format Python files under a target directory")
    parser.add_argument("--target-dir", default=str(Path(__file__).resolve().parent))
    parser.add_argument("--check-only", action="store_true")
    return parser.parse_args()


def get_python_executable(script_dir: Path) -> str:
    repo_root = script_dir.parent
    venv_python = repo_root / ".venv" / "Scripts" / "python.exe"
    if venv_python.exists():
        print(f"Using Python executable from virtual environment: {venv_python}")
        return str(venv_python)

    python_path = shutil.which("python")
    if python_path:
        print(f"Using Python executable from system PATH: {python_path}")
        return python_path

    raise FileNotFoundError("Python executable not found. Create/activate a virtual environment or install Python.")


def main() -> int:
    args = parse_args()
    target_dir = Path(args.target_dir).resolve()
    if not target_dir.exists():
        raise FileNotFoundError(f"Target directory does not exist: {target_dir}")

    script_dir = Path(__file__).resolve().parent
    python_exe = get_python_executable(script_dir)

    black_cmd = [python_exe, "-m", "black", "-q", "--config", str(target_dir / "pyproject.toml")]
    if args.check_only:
        black_cmd.append("--check")

    black_cmd.append(str(target_dir))

    result = subprocess.run(black_cmd, check=False)

    if result.returncode == 0:
        print("Python files are properly formatted.")
    elif result.returncode == 1:
        print("Some Python files need formatting. Run without --check-only to format them.")
    else:
        print("An error occurred while checking/formatting Python files.")

    return result.returncode


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except Exception as exc:  # noqa: BLE001
        print(f"Error: {exc}", file=sys.stderr)
        raise SystemExit(1)
