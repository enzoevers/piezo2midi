import sys
import utils

VERSION = "8.1"


def main() -> int:
    project_root = utils.get_project_root()
    avrdude_dir = (project_root / "avrdude").resolve()

    if avrdude_dir.exists():
        print(f"AVRDUDE folder already exists at {avrdude_dir}")
        return 0

    avrdude_dir.mkdir(parents=True, exist_ok=True)
    archive_path = avrdude_dir / "avrdude.zip"
    avrdude_url = (
        f"https://github.com/avrdudes/avrdude/releases/download/v{VERSION}/" f"avrdude-v{VERSION}-windows-x64.zip"
    )

    print(f"Downloading AVRDUDE from {avrdude_url} to {archive_path}")
    utils.download_with_curl(avrdude_url, archive_path)

    print("Extracting AVRDUDE archive...")
    utils.extract_zip(archive_path, avrdude_dir)

    print("AVRDUDE installed successfully")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except Exception as exc:  # noqa: BLE001
        print(f"Error: {exc}", file=sys.stderr)
        raise SystemExit(1)
