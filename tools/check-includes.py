#!/usr/bin/env python
"""Checks that every quoted #include is written from an include root.

Rule: '#include "X"' is valid only if <root>/X exists for one of the --root folders
(e.g. "Orion/Renderer/Buffer.h" or "oepch.h" from src/OrionEngine). Paths relative to
the including file's folder ("Buffer.h" from Orion/Renderer/) are rejected, including
the .cpp's own header. Angle-bracket includes (<...>) are not checked.

Exit code: 0 if everything is valid, 1 if any include is wrong (the build stops).
With --fix, includes that can be resolved are rewritten in place.
"""

import argparse
import re
import sys
from pathlib import Path

EXTENSIONS = {".h", ".hpp", ".cpp", ".cc", ".inl"}
INCLUDE_RE = re.compile(r'^(\s*#\s*include\s+")([^"]+)(")')


def find_root_relative(path, roots):
    """Returns 'path' written relative to the first root that contains it, or None."""
    for root in roots:
        try:
            return path.relative_to(root).as_posix()
        except ValueError:
            continue
    return None


def check_file(file, roots, fix, repo):
    errors = []
    text = file.read_text(encoding="utf-8")
    lines = text.splitlines(keepends=True)
    changed = False

    for number, line in enumerate(lines, start=1):
        match = INCLUDE_RE.match(line)
        if not match:
            continue
        include = match.group(2)
        if any((root / include).is_file() for root in roots):
            continue

        location = f"{file.relative_to(repo).as_posix()}:{number}"
        local = (file.parent / include).resolve()
        suggestion = find_root_relative(local, roots) if local.is_file() else None

        if suggestion is None:
            errors.append(f'{location}: "{include}" not found under any include root')
        elif fix:
            lines[number - 1] = INCLUDE_RE.sub(lambda m: m.group(1) + suggestion + m.group(3), line, count=1)
            changed = True
            print(f'{location}: "{include}" -> "{suggestion}" (fixed)')
        else:
            errors.append(f'{location}: "{include}" -> use "{suggestion}"')

    if changed:
        with open(file, "w", encoding="utf-8", newline="") as handle:
            handle.write("".join(lines))
    return errors


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--root", action="append", required=True, help="include root folder (repeatable)")
    parser.add_argument("--fix", action="store_true", help="rewrite fixable includes in place")
    parser.add_argument("dirs", nargs="+", help="folders to scan recursively")
    args = parser.parse_args()

    repo = Path.cwd().resolve()
    roots = [Path(root).resolve() for root in args.root]
    files = sorted(
        path for folder in args.dirs for path in Path(folder).resolve().rglob("*") if path.suffix in EXTENSIONS
    )

    errors = []
    for file in files:
        errors.extend(check_file(file, roots, args.fix, repo))

    for error in errors:
        print(error, file=sys.stderr)
    if errors:
        print(f"{len(errors)} include(s) not written from an include root. Build stopped.", file=sys.stderr)
        if not args.fix:
            print("Run tools/check-includes.py with --fix to rewrite the fixable ones.", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
