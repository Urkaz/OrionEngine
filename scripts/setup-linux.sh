#!/usr/bin/env bash
# Installs the system packages needed to build OrionEngine on Linux and initializes the git
# submodules. Supports apt (Debian/Ubuntu), dnf (Fedora) and pacman (Arch).
# Library dependencies (spdlog, GLFW, glm, Catch2) are fetched by CPM at configure time.

set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

SUDO=""
if [[ ${EUID} -ne 0 ]]; then
    SUDO="sudo"
fi

if command -v apt-get > /dev/null; then
    PACKAGES=(
        # Toolchain
        build-essential cmake ninja-build git python3 pkg-config gdb
        clang lld llvm clangd clang-tidy clang-format cmake-format doxygen graphviz
        # Sanitizer runtimes for clang (linux-clang-asan preset) and coverage reports
        libclang-rt-dev gcovr
        # OpenGL + GLFW (X11 and Wayland backends)
        libgl-dev libegl-dev
        libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libxext-dev
        libwayland-dev libxkbcommon-dev wayland-protocols
        # Tracy profiler GUI
        libdbus-1-dev libtbb-dev
    )
    ${SUDO} apt-get update
    ${SUDO} apt-get install -y --no-install-recommends "${PACKAGES[@]}"
elif command -v dnf > /dev/null; then
    PACKAGES=(
        gcc gcc-c++ make cmake ninja-build git python3 pkgconf-pkg-config gdb
        clang lld llvm clang-tools-extra cmake-format doxygen graphviz
        compiler-rt gcovr
        mesa-libGL-devel mesa-libEGL-devel
        libX11-devel libXrandr-devel libXinerama-devel libXcursor-devel libXi-devel libXext-devel
        wayland-devel libxkbcommon-devel wayland-protocols-devel
        dbus-devel tbb-devel
    )
    ${SUDO} dnf install -y "${PACKAGES[@]}"
elif command -v pacman > /dev/null; then
    PACKAGES=(
        base-devel cmake ninja git python pkgconf gdb
        clang lld llvm compiler-rt doxygen graphviz
        libglvnd
        libx11 libxrandr libxinerama libxcursor libxi libxext
        wayland libxkbcommon wayland-protocols
        dbus onetbb
    )
    ${SUDO} pacman -S --needed --noconfirm "${PACKAGES[@]}"
    echo "[setup-linux] cmake-format and gcovr are not in the official Arch repos: pip install --user cmakelang gcovr"
else
    echo "[setup-linux] Unsupported package manager. Install the equivalents manually." >&2
    exit 1
fi

echo "[setup-linux] Initializing git submodules..."
git -C "${ROOT}" submodule update --init --recursive

echo "[setup-linux] Done. Configure with: cmake --preset linux-clang-debug"
