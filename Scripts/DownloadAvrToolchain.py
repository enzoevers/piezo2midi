import sys
import utils

AVR_TC_VERSION = "4.0.0.52"
MAKE_VERSION = "3.81"

AVR_TC_URL = (
    f"https://ww1.microchip.com/downloads/aemDocuments/documents/DEV/ProductDocuments/"
    f"SoftwareTools/avr8-gnu-toolchain-{AVR_TC_VERSION}-win32.any.x86_64.zip"
)
MAKE_BIN_URL = (
    f"https://sourceforge.net/projects/gnuwin32/files/make/{MAKE_VERSION}/make-{MAKE_VERSION}-bin.zip/download"
)
MAKE_DEP_URL = (
    f"https://sourceforge.net/projects/gnuwin32/files/make/{MAKE_VERSION}/make-{MAKE_VERSION}-dep.zip/download"
)


def main() -> int:
    project_root = utils.get_project_root()
    toolchain_dir = (project_root / "avr-toolchain").resolve()

    if toolchain_dir.exists():
        print(f"AVR toolchain folder already exists at {toolchain_dir}")
        return 0

    toolchain_dir.mkdir(parents=True, exist_ok=True)

    avr8_dir = toolchain_dir / "avr8-gnu-toolchain"
    avr8_dir.mkdir(parents=True, exist_ok=True)

    avr8_zip = toolchain_dir / "avr8-gnu-toolchain.zip"

    print(f"Downloading AVR toolchain from {AVR_TC_URL} to {avr8_zip}")
    utils.download_with_curl(AVR_TC_URL, avr8_zip)
    print(f"Extracting AVR toolchain {avr8_zip} to {avr8_dir}")
    utils.extract_zip_strip_components(avr8_zip, avr8_dir, strip_components=1)

    gnuwin32_dir = toolchain_dir / "gnuwin32"
    gnuwin32_dir.mkdir(parents=True, exist_ok=True)

    make_bin_zip = toolchain_dir / "make-bin.zip"
    print(f"Downloading make binaries from {MAKE_BIN_URL} to {make_bin_zip}")
    utils.download_with_curl(MAKE_BIN_URL, make_bin_zip)
    print(f"Extracting make binaries {make_bin_zip} to {gnuwin32_dir}")
    utils.extract_zip(make_bin_zip, gnuwin32_dir)

    make_dep_zip = toolchain_dir / "make-dep.zip"
    print(f"Downloading make dependencies from {MAKE_DEP_URL} to {make_dep_zip}")
    utils.download_with_curl(MAKE_DEP_URL, make_dep_zip)
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
