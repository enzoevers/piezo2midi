import subprocess
import sys
import utils

VERSION = "2025-12-13"


def main() -> int:
    project_root = utils.get_project_root()
    msys_tool_path = (project_root / "msys2-tool").resolve()

    if msys_tool_path.exists():
        print(f"MSYS2 tool folder already exists at {msys_tool_path}")
        return 0

    msys_tool_path.mkdir(parents=True, exist_ok=True)
    installer_path = msys_tool_path / "msys2.exe"
    msys_url = (
        f"https://github.com/msys2/msys2-installer/releases/download/{VERSION}/"
        f"msys2-base-x86_64-{VERSION.replace('-', '')}.sfx.exe"
    )

    print(f"Downloading MSYS2 installer from {msys_url} to {installer_path}")
    utils.download_with_curl(msys_url, installer_path)

    print("Extracting MSYS2 archive...")
    result = subprocess.run(
        [str(installer_path), "-y", f"-o{msys_tool_path}"],
        check=False,
    )
    if result.returncode != 0:
        raise RuntimeError("Failed to extract MSYS2 installer")

    print("MSYS2 installed successfully")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except Exception as exc:  # noqa: BLE001
        print(f"Error: {exc}", file=sys.stderr)
        raise SystemExit(1)
