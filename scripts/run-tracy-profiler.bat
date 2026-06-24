@echo off
setlocal

rem Builds and launches the Tracy profiler GUI ("server"). Per Tracy's own docs, this tool is
rem configured and built as a standalone CMake project, independent from the engine's build tree.

set "ROOT=%~dp0.."
set "PROFILER_SRC=%ROOT%\src\ThirdParty\Tracy\source\profiler"
set "BUILD_DIR=%ROOT%\build-tracy-profiler"
set "CPM_SOURCE_CACHE=%ROOT%\build-tracy-profiler-cache"

if not exist "%BUILD_DIR%\CMakeCache.txt" (
    echo [tracy-profiler] Configuring standalone build in "%BUILD_DIR%"...
    cmake -S "%PROFILER_SRC%" -B "%BUILD_DIR%" -DCMAKE_BUILD_TYPE=Release
    if errorlevel 1 goto :error
)

echo [tracy-profiler] Building...
cmake --build "%BUILD_DIR%" --config Release --parallel
if errorlevel 1 goto :error

set "EXE=%BUILD_DIR%\Release\tracy-profiler.exe"
if not exist "%EXE%" set "EXE=%BUILD_DIR%\tracy-profiler.exe"

if not exist "%EXE%" (
    echo [tracy-profiler] Build succeeded but tracy-profiler.exe was not found.
    goto :error
)

echo [tracy-profiler] Launching "%EXE%"...
start "" "%EXE%"
goto :eof

:error
echo [tracy-profiler] Failed. See output above.
exit /b 1
