#!/usr/bin/env python3
"""Generate JSON reports with flash/RAM usage per library and total application usage.

This script is toolchain-agnostic as long as the selected size tool supports
GNU size style output for -A (SysV format), e.g. avr-size, arm-none-eabi-size,
or size.
"""

from __future__ import annotations

import argparse
import datetime as dt
import json
import re
import shutil
import shlex
import subprocess
import sys
from pathlib import Path
from typing import Dict, Iterable, List, Tuple

DEFAULT_FLASH_PATTERNS = [
    ".text",
    ".text.*",
    ".gnu.linkonce.t.*",
    ".rodata",
    ".rodata.*",
    ".gnu.linkonce.r.*",
    ".progmem",
    ".progmem.*",
    ".jumptables*",
    ".init*",
    ".fini*",
    ".vectors",
    ".ctors",
    ".dtors",
]

DEFAULT_RAM_PATTERNS = [
    ".data",
    ".data.*",
    ".sdata",
    ".sdata.*",
    ".bss",
    ".bss.*",
    ".sbss",
    ".sbss.*",
    ".noinit",
    ".noinit.*",
    "COMMON",
    "*COM*",
]


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description=(
            "Analyze binary resource usage (flash and RAM) for libraries/modules "
            "and final application, then write a JSON report."
        )
    )
    parser.add_argument(
        "--build-dir",
        type=Path,
        default=Path("build"),
        help="Build directory to scan (default: build)",
    )
    parser.add_argument(
        "--elf",
        type=Path,
        default=None,
        help="Path to final ELF/executable to analyze. If omitted, auto-discovery is used.",
    )
    parser.add_argument(
        "--output",
        type=Path,
        default=Path("build") / "resource_usage_report.json",
        help="Path to the JSON output file (default: build/resource_usage_report.json)",
    )
    parser.add_argument(
        "--size-tool",
        default="avr-size",
        help="Size tool executable (default: avr-size, fallback: size)",
    )
    parser.add_argument(
        "--top",
        type=int,
        default=5,
        help="Number of biggest parts to list per library/module (default: 5)",
    )
    parser.add_argument(
        "--flash-sections",
        default=",".join(DEFAULT_FLASH_PATTERNS),
        help="Comma-separated glob patterns for sections counted as flash",
    )
    parser.add_argument(
        "--ram-sections",
        default=",".join(DEFAULT_RAM_PATTERNS),
        help="Comma-separated glob patterns for sections counted as RAM",
    )
    parser.add_argument(
        "--include-symbol-sizes",
        action="store_true",
        help="Also collect symbol size breakdowns using nm and include them in JSON",
    )
    parser.add_argument(
        "--symbol-tool",
        default="avr-nm",
        help=("nm executable to use for symbol sizes (default: avr-nm)"),
    )
    parser.add_argument(
        "--symbols-top",
        type=int,
        default=20,
        help="Number of biggest symbols to keep per artifact/module (default: 20)",
    )
    args = parser.parse_args()

    print("Running the script with the following arguments:")
    for arg, value in vars(args).items():
        print(f"  {arg}: {value}")

    return args


def split_patterns(value: str, delimiter: str = ",") -> List[str]:
    return [item.strip() for item in value.split(delimiter) if item.strip()]


def find_tool(preferred: str) -> str:
    resolved = shutil.which(preferred)
    if resolved:
        return resolved

    raise FileNotFoundError(f"Could not find size tool '{preferred}' in PATH.")


def parse_cmake_cache(cache_path: Path) -> Dict[str, str]:
    entries: Dict[str, str] = {}
    if not cache_path.exists():
        return entries

    for raw_line in cache_path.read_text(encoding="utf-8", errors="ignore").splitlines():
        line = raw_line.strip()
        if not line or line.startswith("#") or line.startswith("//"):
            continue

        # Format: KEY:TYPE=VALUE
        if ":" not in line or "=" not in line:
            continue

        key_part, value = line.split("=", 1)
        key = key_part.split(":", 1)[0].strip()
        if not key:
            continue
        entries[key] = value.strip()

    return entries


def parse_cmake_set_file(path: Path) -> Dict[str, str]:
    values: Dict[str, str] = {}
    if not path.exists():
        return values

    pattern = re.compile(r"^\s*set\(([^\s\)]+)\s+\"?(.*?)\"?\)\s*$")
    for raw_line in path.read_text(encoding="utf-8", errors="ignore").splitlines():
        line = raw_line.strip()
        if not line or line.startswith("#"):
            continue

        match = pattern.match(line)
        if not match:
            continue

        key = match.group(1).strip()
        value = match.group(2).strip()
        values[key] = value

    return values


def collect_flag_files(build_dir: Path) -> List[Dict[str, str]]:
    items: List[Dict[str, str]] = []
    for path in sorted(build_dir.rglob("flags.make")):
        relative_path = str(path.relative_to(build_dir).as_posix())
        for raw_line in path.read_text(encoding="utf-8", errors="ignore").splitlines():
            line = raw_line.strip()
            if "=" not in line:
                continue

            key, value = line.split("=", 1)
            key = key.strip()
            value = value.strip()
            if not key.endswith("_FLAGS"):
                continue
            if not value:
                continue

            items.append(
                {
                    "file": relative_path,
                    "variable": key,
                    "value": value,
                }
            )
    return items


def extract_linker_flags_from_command(command: str) -> List[str]:
    try:
        tokens = shlex.split(command, posix=False)
    except ValueError:
        tokens = command.split()

    flags: List[str] = []
    for token in tokens:
        if token.startswith("-"):
            flags.append(token)
    return flags


def collect_link_commands(build_dir: Path) -> List[Dict[str, object]]:
    commands: List[Dict[str, object]] = []
    for path in sorted(build_dir.rglob("link.txt")):
        relative_path = str(path.relative_to(build_dir).as_posix())
        for raw_line in path.read_text(encoding="utf-8", errors="ignore").splitlines():
            line = raw_line.strip()
            if not line:
                continue

            commands.append(
                {
                    "file": relative_path,
                    "command": line,
                    "flags": extract_linker_flags_from_command(line),
                }
            )
    return commands


def collect_build_metadata(build_dir: Path) -> Dict[str, object]:
    CMakeCache_path = build_dir / "CMakeCache.txt"
    CMakeCXXCompiler_path = next(
        Path(build_dir / "CMakeFiles").rglob("CMakeCXXCompiler.cmake", case_sensitive=True), None
    )
    CMakeCCompiler_path = next(Path(build_dir / "CMakeFiles").rglob("CMakeCCompiler.cmake", case_sensitive=True), None)

    print("Using files for build metadata collection:")
    print(f"  CMakeCache.txt: {CMakeCache_path}")
    print(f"  CMakeCXXCompiler.cmake: {CMakeCXXCompiler_path}")
    print(f"  CMakeCCompiler.cmake: {CMakeCCompiler_path}")

    cache = parse_cmake_cache(CMakeCache_path)
    cxx_cfg = parse_cmake_set_file(CMakeCXXCompiler_path) if CMakeCXXCompiler_path else {}
    c_cfg = parse_cmake_set_file(CMakeCCompiler_path) if CMakeCCompiler_path else {}

    print("Parsed build metadata")

    c_compiler = cache.get("CMAKE_C_COMPILER") or c_cfg.get("CMAKE_C_COMPILER")
    cxx_compiler = cache.get("CMAKE_CXX_COMPILER") or cxx_cfg.get("CMAKE_CXX_COMPILER")
    compiler_path = cxx_compiler or c_compiler

    compiler_version = (
        cache.get("CMAKE_CXX_COMPILER_VERSION")
        or cxx_cfg.get("CMAKE_CXX_COMPILER_VERSION")
        or cache.get("CMAKE_C_COMPILER_VERSION")
        or c_cfg.get("CMAKE_C_COMPILER_VERSION")
    )

    compile_flags_cache = {
        key: value
        for key, value in cache.items()
        if (key.startswith("CMAKE_C_FLAGS") or key.startswith("CMAKE_CXX_FLAGS") or key.startswith("CMAKE_ASM_FLAGS"))
        and not key.endswith("-ADVANCED")
        and value
    }
    linker_flags_cache = {
        key: value
        for key, value in cache.items()
        if key.startswith("CMAKE_EXE_LINKER_FLAGS") and not key.endswith("-ADVANCED") and value
    }

    return {
        "build_system": {
            "generator": cache.get("CMAKE_GENERATOR"),
            "build_tool": cache.get("CMAKE_MAKE_PROGRAM"),
            "cache_present": (build_dir / "CMakeCache.txt").exists(),
        },
        "compiler": {
            "c": c_compiler,
            "cxx": cxx_compiler,
            "detected_primary": compiler_path,
            "version": compiler_version,
        },
        "compile_flags": {
            "from_cmake_cache": compile_flags_cache,
            "from_flags_make": collect_flag_files(build_dir),
        },
        "linker_flags": {
            "from_cmake_cache": linker_flags_cache,
            "from_link_commands": collect_link_commands(build_dir),
        },
    }


def is_probable_elf(path: Path) -> bool:
    try:
        with path.open("rb") as file:
            return file.read(4) == b"\x7fELF"
    except OSError:
        return False


def get_largest_file(files: List[Path]) -> Path:
    return max(files, key=lambda p: p.stat().st_size)


def discover_elf(build_dir: Path) -> Path:
    # CMake generated a file that contains the string "CompilerId". This can be ignored.
    elf_candidates = [p for p in build_dir.rglob("*.elf") if p.is_file() and "CompilerId" not in str(p)]
    if elf_candidates:
        return get_largest_file(elf_candidates)

    # Generic fallback for toolchains producing extension-less executable outputs.
    bin_candidates: List[Path] = []
    for p in build_dir.rglob("*"):
        if not p.is_file():
            continue
        if p.suffix:
            continue
        if "CompilerId" in str(p):
            continue
        if is_probable_elf(p):
            bin_candidates.append(p)

    if bin_candidates:
        return get_largest_file(bin_candidates)

    raise FileNotFoundError("Could not auto-discover final ELF/executable under build dir. " "Pass --elf explicitly.")


def run_size(size_tool: str, target: Path) -> str:
    command = [size_tool, "-A", str(target)]
    process = subprocess.run(
        command,
        capture_output=True,
        text=True,
        check=False,
    )
    if process.returncode != 0:
        raise RuntimeError(f"Failed running '{' '.join(command)}'. stderr:\n{process.stderr.strip()}")
    return process.stdout


def run_nm(symbol_tool: str, target: Path) -> str:
    command = [
        symbol_tool,
        "-C",
        "--print-size",
        "--size-sort",
        "--radix=d",
        str(target),
    ]
    process = subprocess.run(
        command,
        capture_output=True,
        text=True,
        check=False,
    )
    if process.returncode != 0:
        raise RuntimeError(f"Failed running '{' '.join(command)}'. stderr:\n{process.stderr.strip()}")
    return process.stdout


def parse_size_a_output(output: str) -> Dict[str, Dict[str, int]]:
    """Parse GNU size -A output into {input_name: {section: bytes}}."""
    parsed: Dict[str, Dict[str, int]] = {}
    current_input: str | None = None

    for raw_line in output.splitlines():
        line = raw_line.strip()
        if not line:
            continue

        if line.endswith(":"):
            current_input = line[:-1].strip()
            parsed.setdefault(current_input, {})
            continue

        lower = line.lower()
        if lower.startswith("section") or lower.startswith("total"):
            continue

        parts = line.split()
        if len(parts) < 2:
            continue

        section = parts[0]
        try:
            size_value = int(parts[1], 0)
        except ValueError:
            continue

        if current_input is None:
            current_input = "<unknown>"
            parsed.setdefault(current_input, {})

        parsed[current_input][section] = parsed[current_input].get(section, 0) + size_value

    return parsed


def matches_any_glob(section_name: str, patterns: Iterable[str]) -> bool:
    from fnmatch import fnmatchcase

    return any(fnmatchcase(section_name, pattern) for pattern in patterns)


def section_totals_to_usage(
    sections: Dict[str, int], flash_patterns: List[str], ram_patterns: List[str]
) -> Dict[str, object]:
    flash_bytes = 0
    ram_bytes = 0
    unknown_sections: Dict[str, int] = {}

    for section, size_value in sections.items():
        if matches_any_glob(section, flash_patterns):
            flash_bytes += size_value
        elif matches_any_glob(section, ram_patterns):
            ram_bytes += size_value
        else:
            unknown_sections[section] = size_value

    return {
        "flash_bytes": flash_bytes,
        "ram_bytes": ram_bytes,
        "sections": dict(sorted(sections.items())),
        "unclassified_sections": dict(sorted(unknown_sections.items())),
    }


def library_name_from_archive(archive_path: Path) -> str:
    name = archive_path.name
    if name.startswith("lib") and name.endswith(".a"):
        return name[3:-2]
    if name.endswith(".a"):
        return name[:-2]
    return name


def extract_member_name(input_name: str, archive_path: Path) -> str:
    cleaned = re.sub(r"\s+\(ex\s+.+\)$", "", input_name).strip()

    archive_name = archive_path.name
    if cleaned.startswith(f"{archive_name}:"):
        return cleaned.split(":", 1)[1].strip()

    # Alternate style used by some size variants: libX.a(member.o)
    prefix = f"{archive_name}("
    if cleaned.startswith(prefix) and cleaned.endswith(")"):
        return cleaned[len(prefix) : -1].strip()

    return cleaned


def parse_nm_output(output: str) -> List[Dict[str, object]]:
    symbols: List[Dict[str, object]] = []

    # Format expected with --print-size --radix=d:
    # <addr> <size> <type> <name>
    pattern = re.compile(r"^\s*(\d+)\s+(\d+)\s+([A-Za-z?])\s+(.+?)\s*$")
    for raw_line in output.splitlines():
        line = raw_line.strip()
        if not line:
            continue
        if line.endswith(":"):
            # Archive/member header line.
            continue

        match = pattern.match(line)
        if not match:
            continue

        size_bytes = int(match.group(2), 10)
        if size_bytes <= 0:
            continue

        symbols.append(
            {
                "name": match.group(4),
                "size_bytes": size_bytes,
                "type": match.group(3),
            }
        )

    symbols.sort(key=lambda item: int(item["size_bytes"]), reverse=True)
    return symbols


def classify_symbol_type(symbol_type: str) -> str:
    t = symbol_type.lower()
    if t in {"t", "r", "w"}:
        return "flash"
    if t in {"d", "b", "g", "s", "c", "v"}:
        return "ram"
    return "other"


def build_symbol_report_for_target(symbol_tool: str, target: Path, top_n: int) -> Dict[str, object]:
    symbols = parse_nm_output(run_nm(symbol_tool, target))

    flash_sum = 0
    ram_sum = 0
    other_sum = 0
    for sym in symbols:
        cls = classify_symbol_type(str(sym["type"]))
        if cls == "flash":
            flash_sum += int(sym["size_bytes"])
        elif cls == "ram":
            ram_sum += int(sym["size_bytes"])
        else:
            other_sum += int(sym["size_bytes"])

    return {
        "artifact": str(target.as_posix()),
        "symbol_count": len(symbols),
        "sum_by_symbol_type": {
            "flash_bytes": flash_sum,
            "ram_bytes": ram_sum,
            "other_bytes": other_sum,
            "total_bytes": flash_sum + ram_sum + other_sum,
        },
        "biggest_symbols": symbols[:top_n],
    }


def build_module_symbol_report(
    objects: List[Path], build_dir: Path, symbol_tool: str, top_n: int
) -> List[Dict[str, object]]:
    module_symbols: Dict[str, List[Dict[str, object]]] = {}

    for obj in objects:
        module = infer_module_name_from_object(obj, build_dir)
        symbols = parse_nm_output(run_nm(symbol_tool, obj))
        if not symbols:
            continue

        for sym in symbols:
            symbol_copy = dict(sym)
            symbol_copy["source"] = str(obj.relative_to(build_dir).as_posix())
            module_symbols.setdefault(module, []).append(symbol_copy)

    result: List[Dict[str, object]] = []
    for module_name, symbols in module_symbols.items():
        symbols.sort(key=lambda item: int(item["size_bytes"]), reverse=True)

        flash_sum = 0
        ram_sum = 0
        other_sum = 0
        for sym in symbols:
            cls = classify_symbol_type(str(sym["type"]))
            if cls == "flash":
                flash_sum += int(sym["size_bytes"])
            elif cls == "ram":
                ram_sum += int(sym["size_bytes"])
            else:
                other_sum += int(sym["size_bytes"])

        result.append(
            {
                "name": module_name,
                "symbol_count": len(symbols),
                "sum_by_symbol_type": {
                    "flash_bytes": flash_sum,
                    "ram_bytes": ram_sum,
                    "other_bytes": other_sum,
                    "total_bytes": flash_sum + ram_sum + other_sum,
                },
                "biggest_symbols": symbols[:top_n],
            }
        )

    result.sort(
        key=lambda item: int(item["sum_by_symbol_type"]["total_bytes"]),
        reverse=True,
    )
    return result


def analyze_archives(
    archives: List[Path],
    size_tool: str,
    flash_patterns: List[str],
    ram_patterns: List[str],
    top_n: int,
) -> Dict[str, object]:
    libraries: List[Dict[str, object]] = []

    for archive in sorted(archives):
        size_output = run_size(size_tool, archive)
        members = parse_size_a_output(size_output)

        member_entries: List[Dict[str, object]] = []
        lib_flash = 0
        lib_ram = 0

        for input_name, sections in members.items():
            member_name = extract_member_name(input_name, archive)
            usage = section_totals_to_usage(sections, flash_patterns, ram_patterns)
            part_total = usage["flash_bytes"] + usage["ram_bytes"]

            entry = {
                "name": member_name,
                "flash_bytes": usage["flash_bytes"],
                "ram_bytes": usage["ram_bytes"],
                "total_bytes": part_total,
                "sections": usage["sections"],
            }
            member_entries.append(entry)
            lib_flash += usage["flash_bytes"]
            lib_ram += usage["ram_bytes"]

        member_entries.sort(key=lambda item: int(item["total_bytes"]), reverse=True)

        libraries.append(
            {
                "name": library_name_from_archive(archive),
                "archive": str(archive.as_posix()),
                "flash_bytes": lib_flash,
                "ram_bytes": lib_ram,
                "total_bytes": lib_flash + lib_ram,
                "biggest_parts": member_entries[:top_n],
            }
        )

    libraries.sort(key=lambda item: int(item["total_bytes"]), reverse=True)

    return {
        "mode": "archives",
        "count": len(libraries),
        "items": libraries,
    }


def infer_module_name_from_object(obj_path: Path, build_dir: Path) -> str:
    relative = obj_path.relative_to(build_dir)
    return relative.parts[0] if relative.parts else "unknown"


def analyze_objects_as_modules(
    objects: List[Path],
    build_dir: Path,
    size_tool: str,
    flash_patterns: List[str],
    ram_patterns: List[str],
    top_n: int,
) -> Dict[str, object]:
    modules: Dict[str, Dict[str, object]] = {}

    for obj in sorted(objects):
        parsed = parse_size_a_output(run_size(size_tool, obj))
        if not parsed:
            continue

        # For plain object files there is typically one input block.
        obj_sections = next(iter(parsed.values()))
        usage = section_totals_to_usage(obj_sections, flash_patterns, ram_patterns)
        module_name = infer_module_name_from_object(obj, build_dir)

        module = modules.setdefault(
            module_name,
            {
                "name": module_name,
                "flash_bytes": 0,
                "ram_bytes": 0,
                "parts": [],
            },
        )

        part_total = usage["flash_bytes"] + usage["ram_bytes"]
        module["flash_bytes"] += usage["flash_bytes"]
        module["ram_bytes"] += usage["ram_bytes"]
        module["parts"].append(
            {
                "name": str(obj.relative_to(build_dir).as_posix()),
                "flash_bytes": usage["flash_bytes"],
                "ram_bytes": usage["ram_bytes"],
                "total_bytes": part_total,
                "sections": usage["sections"],
            }
        )

    module_items: List[Dict[str, object]] = []
    for module in modules.values():
        parts = module["parts"]
        parts.sort(key=lambda item: int(item["total_bytes"]), reverse=True)

        module_items.append(
            {
                "name": module["name"],
                "flash_bytes": module["flash_bytes"],
                "ram_bytes": module["ram_bytes"],
                "total_bytes": module["flash_bytes"] + module["ram_bytes"],
                "biggest_parts": parts[:top_n],
            }
        )

    module_items.sort(key=lambda item: int(item["total_bytes"]), reverse=True)

    return {
        "mode": "object_modules",
        "count": len(module_items),
        "items": module_items,
    }


def analyze_application(
    elf_path: Path, size_tool: str, flash_patterns: List[str], ram_patterns: List[str]
) -> Dict[str, object]:
    parsed = parse_size_a_output(run_size(size_tool, elf_path))
    if not parsed:
        raise RuntimeError(f"No section data parsed for application: {elf_path}")

    # size -A on executable typically yields one input block.
    app_sections = next(iter(parsed.values()))
    usage = section_totals_to_usage(app_sections, flash_patterns, ram_patterns)

    return {
        "path": str(elf_path.as_posix()),
        "flash_bytes": usage["flash_bytes"],
        "ram_bytes": usage["ram_bytes"],
        "total_bytes": usage["flash_bytes"] + usage["ram_bytes"],
        "sections": usage["sections"],
        "unclassified_sections": usage["unclassified_sections"],
    }


def build_compact_summary(
    libraries_report: Dict[str, object], app_report: Dict[str, object], top_n: int = 5
) -> Dict[str, object]:
    items = libraries_report.get("items", [])
    if not isinstance(items, list):
        items = []

    app_flash = int(app_report.get("flash_bytes", 0))
    app_ram = int(app_report.get("ram_bytes", 0))
    app_total = int(app_report.get("total_bytes", 0))

    def percent(value: int, total: int) -> float:
        if total <= 0:
            return 0.0
        return round((value * 100.0) / total, 2)

    sorted_items = sorted(
        items,
        key=lambda item: int(item.get("total_bytes", 0)),
        reverse=True,
    )

    top_entries: List[Dict[str, object]] = []
    for item in sorted_items[:top_n]:
        flash = int(item.get("flash_bytes", 0))
        ram = int(item.get("ram_bytes", 0))
        total = int(item.get("total_bytes", flash + ram))

        top_entries.append(
            {
                "name": str(item.get("name", "unknown")),
                "flash_bytes": flash,
                "ram_bytes": ram,
                "total_bytes": total,
                "flash_pct_of_app": percent(flash, app_flash),
                "ram_pct_of_app": percent(ram, app_ram),
                "total_pct_of_app": percent(total, app_total),
            }
        )

    return {
        "mode": libraries_report.get("mode", "none"),
        "library_or_module_count": int(libraries_report.get("count", 0)),
        "application": {
            "flash_bytes": app_flash,
            "ram_bytes": app_ram,
            "total_bytes": app_total,
        },
        "top_contributors": top_entries,
    }


def main() -> int:
    args = parse_args()

    build_dir = args.build_dir.resolve()
    if not build_dir.exists():
        raise FileNotFoundError(f"Build directory does not exist: {build_dir}")

    flash_patterns = split_patterns(args.flash_sections, delimiter=",")
    ram_patterns = split_patterns(args.ram_sections, delimiter=",")

    size_tool = find_tool(args.size_tool)
    print(f"Using size tool: {size_tool}")

    symbol_tool = find_tool(args.symbol_tool)
    print(f"Using symbol tool: {symbol_tool}")

    elf_path = args.elf.resolve() if args.elf else discover_elf(build_dir)
    if not elf_path.exists():
        raise FileNotFoundError(f"ELF/executable does not exist: {elf_path}")

    print(f"Using ELF/executable: {elf_path}")

    archives = [p for p in build_dir.rglob("*.a") if p.is_file()]

    print("Found the following archives:")
    for archive in archives:
        print(f"  {archive}")

    objects = [
        p
        for p in build_dir.rglob("*")
        if p.is_file() and p.suffix.lower() in {".o", ".obj"} and "CompilerId" not in str(p)
    ]

    print("Found the following object files:")
    for obj in objects:
        print(f"  {obj}")

    if archives:
        libraries_report = analyze_archives(archives, size_tool, flash_patterns, ram_patterns, args.top)
        print(f"Analyzed {libraries_report.get('count', 0)} archives for resource usage.")
    elif objects:
        libraries_report = analyze_objects_as_modules(
            objects, build_dir, size_tool, flash_patterns, ram_patterns, args.top
        )
        print(f"Analyzed {libraries_report.get('count', 0)} object files as modules for resource usage.")
    else:
        libraries_report = {
            "mode": "none",
            "count": 0,
            "items": [],
            "note": "No .a, .o, or .obj files found in build directory.",
        }
        print("No archives or object files found; skipping library/module analysis.")

    print("Analyzing final application resource usage")
    app_report = analyze_application(elf_path, size_tool, flash_patterns, ram_patterns)

    print("Collecting build metadata")
    build_metadata = collect_build_metadata(build_dir)

    symbol_report: Dict[str, object] | None = None
    if args.include_symbol_sizes:
        print("Collecting symbol sizes for final application")
        symbol_report_for_application = build_symbol_report_for_target(symbol_tool, elf_path, args.symbols_top)
        symbol_report = {
            "enabled": True,
            "symbol_tool": symbol_tool,
            "application": symbol_report_for_application,
        }

        if libraries_report.get("mode") == "archives":
            print("Collecting symbol sizes for libraries")
            library_items = libraries_report.get("items", [])
            if isinstance(library_items, list):
                entries: List[Dict[str, object]] = []
                for item in library_items:
                    if not isinstance(item, dict):
                        continue
                    archive = item.get("archive")
                    if not isinstance(archive, str):
                        continue
                    entries.append(
                        {
                            "name": str(item.get("name", "unknown")),
                            "report": build_symbol_report_for_target(symbol_tool, Path(archive), args.symbols_top),
                        }
                    )
                symbol_report["libraries"] = entries
        elif libraries_report.get("mode") == "object_modules":
            print("Collecting symbol sizes for modules")
            symbol_report["modules"] = build_module_symbol_report(objects, build_dir, symbol_tool, args.symbols_top)

    generated_at = dt.datetime.now(dt.timezone.utc).isoformat()
    report = {
        "schema_version": 1,
        "generated_at_utc": generated_at,
        "size_tool": size_tool,
        "build_dir": str(build_dir.as_posix()),
        "classification": {
            "flash_section_patterns": flash_patterns,
            "ram_section_patterns": ram_patterns,
        },
        "build_metadata": build_metadata,
        "libraries": libraries_report,
        "application": app_report,
        "summary": build_compact_summary(libraries_report, app_report),
    }
    if symbol_report is not None:
        report["symbol_sizes"] = symbol_report

    output_path = args.output.resolve()
    output_path.parent.mkdir(parents=True, exist_ok=True)
    output_path.write_text(json.dumps(report, indent=2), encoding="utf-8")

    print(f"Resource usage report written to: {output_path}")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except Exception as exc:  # noqa: BLE001 - CLI entrypoint should report all errors
        print(f"Error: {exc}", file=sys.stderr)
        raise SystemExit(1)
