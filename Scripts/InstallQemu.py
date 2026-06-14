import subprocess
import sys
import utils


def main() -> int:
    project_root = utils.get_project_root()
    msys_tool_path = (project_root / "msys2-tool").resolve()
    bash_path = msys_tool_path / "msys64" / "usr" / "bin" / "bash"

    exe_path = bash_path if bash_path.exists() else bash_path.with_suffix(".exe")
    if not exe_path.exists():
        raise FileNotFoundError(f"MSYS2 bash not found at {exe_path}. Run DownloadAndInstallMsys.py first.")

    cmd = [
        str(exe_path),
        "-lc",
        "pacman --noconfirm -S mingw-w64-ucrt-x86_64-qemu",
    ]
    result = subprocess.run(cmd, check=False)
    return result.returncode


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except Exception as exc:  # noqa: BLE001
        print(f"Error: {exc}", file=sys.stderr)
        raise SystemExit(1)
