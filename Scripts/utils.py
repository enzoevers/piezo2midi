from pathlib import Path
import shutil
import subprocess
import zipfile


def get_project_root() -> Path:
    return Path(__file__).resolve().parent.parent


def download_with_curl(url: str, destination: Path) -> None:
    curl = shutil.which("curl.exe") or shutil.which("curl")
    if not curl:
        raise FileNotFoundError("curl was not found in PATH")

    result = subprocess.run(
        [curl, "-L", "--fail", "--show-error", "-o", str(destination), url],
        check=False,
    )
    if result.returncode != 0:
        raise RuntimeError(f"curl failed downloading {url} (exit code {result.returncode})")


def extract_zip(zip_path: Path, target_dir: Path) -> None:
    with zipfile.ZipFile(zip_path, "r") as zip_ref:
        zip_ref.extractall(target_dir)


def extract_zip_strip_components(zip_path: Path, target_dir: Path, strip_components: int) -> None:
    with zipfile.ZipFile(zip_path, "r") as zip_ref:
        for member in zip_ref.infolist():
            parts = Path(member.filename).parts
            if len(parts) <= strip_components:
                continue

            relative = Path(*parts[strip_components:])
            destination = target_dir / relative

            if member.is_dir():
                destination.mkdir(parents=True, exist_ok=True)
                continue

            destination.parent.mkdir(parents=True, exist_ok=True)
            with zip_ref.open(member, "r") as src, destination.open("wb") as dst:
                shutil.copyfileobj(src, dst)
