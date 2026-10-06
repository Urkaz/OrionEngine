#!/usr/bin/env bash
# Installs the VS Code extensions recommended in .vscode/extensions.json.

set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

if ! command -v code > /dev/null; then
    echo "[vscode-extensions] 'code' command not found. Is VS Code installed and in PATH?" >&2
    exit 1
fi

mapfile -t EXTENSIONS < <(python3 -c "
import json, re, sys
text = open(sys.argv[1]).read()
text = re.sub(r'^\s*//.*$', '', text, flags=re.MULTILINE)  # extensions.json allows comments
print('\n'.join(json.loads(text)['recommendations']))
" "${ROOT}/.vscode/extensions.json")

for ext in "${EXTENSIONS[@]}"; do
    code --install-extension "${ext}"
done

echo "[vscode-extensions] Done. Reload VS Code (Developer: Reload Window) to apply."
