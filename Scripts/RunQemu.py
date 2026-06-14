#!/usr/bin/env python3
from __future__ import annotations

import subprocess
import sys
from pathlib import Path
import utils


def main() -> int:
    project_root = utils.get_project_root()
    msys_tool_path = (project_root / "msys2-tool").resolve()
    qemu_path = msys_tool_path / "msys64" / "ucrt64" / "bin" / "qemu-system-avr"

    exe_path = qemu_path if qemu_path.exists() else qemu_path.with_suffix(".exe")
    if not exe_path.exists():
        raise FileNotFoundError(f"QEMU executable not found at {qemu_path}. Run InstallQemu.py first.")

    result = subprocess.run([str(exe_path), "--version"], check=False)
    return result.returncode


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except Exception as exc:  # noqa: BLE001
        print(f"Error: {exc}", file=sys.stderr)
        raise SystemExit(1)
