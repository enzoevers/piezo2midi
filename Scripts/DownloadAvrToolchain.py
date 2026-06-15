import argparse
import sys
import utils
from pathlib import Path

# https://www.microchip.com/en-us/tools-resources/develop/microchip-studio/gcc-compilers


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Download and install AVR toolchain")
    parser.add_argument("--target-dir", default=str((utils.get_project_root() / "avr-toolchain").resolve()))
    parser.add_argument(
        "--avr8-toolchain-version", default="4.0.0.52", help="Specify the AVR toolchain version to download"
    )
    parser.add_argument("--gnuwin32-version", default="3.81", help="Specify the gnuwin32 version to download")
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    toolchain_dir = Path(args.target_dir).resolve()

    if toolchain_dir.exists():
        print(f"AVR toolchain folder already exists at {toolchain_dir}")
        return 0

    toolchain_dir.mkdir(parents=True, exist_ok=True)

    avr8_dir = toolchain_dir / "avr8-gnu-toolchain"
    avr8_dir.mkdir(parents=True, exist_ok=True)

    avr_tc_url_windows = (
        f"https://ww1.microchip.com/downloads/aemDocuments/documents/DEV/ProductDocuments/"
        f"SoftwareTools/avr8-gnu-toolchain-{args.avr8_toolchain_version}-win32.any.x86_64.zip"
    )
    avr_tc_url_linux = (
        f"https://ww1.microchip.com/downloads/aemDocuments/documents/DEV/ProductDocuments/"
        f"SoftwareTools/avr8-gnu-toolchain-{args.avr8_toolchain_version}-linux.any.x86_64.tar.gz"
    )

    if utils.is_windows():
        avr_tc_url = avr_tc_url_windows
        avr8_compressed = toolchain_dir / "avr8-gnu-toolchain.zip"
    elif utils.is_linux():
        avr_tc_url = avr_tc_url_linux
        avr8_compressed = toolchain_dir / "avr8-gnu-toolchain.tar.gz"

    print(f"Downloading AVR toolchain from {avr_tc_url} to {avr8_compressed}")
    utils.download_with_curl(avr_tc_url, avr8_compressed)
    print(f"Extracting AVR toolchain {avr8_compressed} to {avr8_dir}")
    if utils.is_windows():
        utils.extract_zip_strip_components(avr8_compressed, avr8_dir, strip_components=1)
    elif utils.is_linux():
        utils.extract_tar_gz_strip_components(avr8_compressed, avr8_dir, strip_components=1)

    if utils.is_windows():
        gnuwin32_dir = toolchain_dir / "gnuwin32"
        gnuwin32_dir.mkdir(parents=True, exist_ok=True)

        gnuwin32_dir_url = f"https://sourceforge.net/projects/gnuwin32/files/make/{args.gnuwin32_version}/make-{args.gnuwin32_version}-bin.zip/download"
        make_bin_zip = toolchain_dir / "make-bin.zip"
        print(f"Downloading make binaries from {gnuwin32_dir_url} to {make_bin_zip}")
        utils.download_with_curl(gnuwin32_dir_url, make_bin_zip)
        print(f"Extracting make binaries {make_bin_zip} to {gnuwin32_dir}")
        utils.extract_zip(make_bin_zip, gnuwin32_dir)

        gnuwin32_dep_url = f"https://sourceforge.net/projects/gnuwin32/files/make/{args.gnuwin32_version}/make-{args.gnuwin32_version}-dep.zip/download"
        make_dep_zip = toolchain_dir / "make-dep.zip"
        print(f"Downloading make dependencies from {gnuwin32_dep_url} to {make_dep_zip}")
        utils.download_with_curl(gnuwin32_dep_url, make_dep_zip)
        print(f"Extracting make dependencies {make_dep_zip} to {gnuwin32_dir}")
        utils.extract_zip(make_dep_zip, gnuwin32_dir)

    print("AVR toolchain installed successfully")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except Exception as exc:  # noqa: BLE001
        print(f"Error: {exc}", file=sys.stderr)
        raise SystemExit(1)
