@echo off
setlocal EnableExtensions
chcp 65001 >nul
cd /d "%~dp0"

title RTextJP example launcher

where cmake >nul 2>nul
if errorlevel 1 goto NO_CMAKE

where nmake >nul 2>nul
if not errorlevel 1 goto COMPILER_READY

if exist "%ProgramFiles%\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat" set "VSDEVCMD=%ProgramFiles%\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"
if exist "%ProgramFiles%\Microsoft Visual Studio\2022\Professional\Common7\Tools\VsDevCmd.bat" set "VSDEVCMD=%ProgramFiles%\Microsoft Visual Studio\2022\Professional\Common7\Tools\VsDevCmd.bat"
if exist "%ProgramFiles%\Microsoft Visual Studio\2022\Enterprise\Common7\Tools\VsDevCmd.bat" set "VSDEVCMD=%ProgramFiles%\Microsoft Visual Studio\2022\Enterprise\Common7\Tools\VsDevCmd.bat"
if exist "%ProgramFiles%\Microsoft Visual Studio\2022\BuildTools\Common7\Tools\VsDevCmd.bat" set "VSDEVCMD=%ProgramFiles%\Microsoft Visual Studio\2022\BuildTools\Common7\Tools\VsDevCmd.bat"
if not defined VSDEVCMD goto NO_COMPILER
call "%VSDEVCMD%" -arch=x64 -host_arch=x64 >nul

where nmake >nul 2>nul
if errorlevel 1 goto NO_COMPILER

:COMPILER_READY

echo [RTextJP] exampleをビルドしています。初回だけ少し待ってください...
cmake -G "NMake Makefiles" -S . -B build-launcher -DCMAKE_BUILD_TYPE=Debug -DRTEXTJP_BUILD_EXAMPLES=ON -DRTEXTJP_BUILD_TESTS=OFF -DRTEXTJP_FETCH_RAYLIB=ON
if errorlevel 1 goto BUILD_ERROR

cmake --build build-launcher
if errorlevel 1 goto BUILD_ERROR

set "EXEDIR=%CD%\build-launcher"
set "FONT=%CD%\examples\resources\japanese.ttf"

if not exist "%EXEDIR%\rtextjp_01_hello_japanese.exe" goto BUILD_ERROR

:MENU
cls
echo ======================================================
echo   RTextJP example launcher
echo ======================================================
echo.
echo   1  最初の日本語表示
echo   2  折り返し・禁則処理
echo   3  左・中央・右揃え
echo   4  簡易縦書き
echo   5  日本語入力欄
echo   6  UTF-8解析ビューア
echo   7  日本語文字集合プリセット
echo   0  終了
echo.
set /p "CHOICE=番号を入力してください Enter: "

if "%CHOICE%"=="1" (
    set "APP=rtextjp_01_hello_japanese.exe"
    goto LAUNCH_GUI
)
if "%CHOICE%"=="2" (
    set "APP=rtextjp_02_text_box.exe"
    goto LAUNCH_GUI
)
if "%CHOICE%"=="3" (
    set "APP=rtextjp_03_alignment.exe"
    goto LAUNCH_GUI
)
if "%CHOICE%"=="4" (
    set "APP=rtextjp_04_vertical_writing.exe"
    goto LAUNCH_GUI
)
if "%CHOICE%"=="5" (
    set "APP=rtextjp_05_text_input.exe"
    goto LAUNCH_GUI
)
if "%CHOICE%"=="6" (
    set "APP=rtextjp_06_utf8_tools.exe"
    goto LAUNCH_CONSOLE
)
if "%CHOICE%"=="7" (
    set "APP=rtextjp_07_character_sets.exe"
    goto LAUNCH_GUI
)
if "%CHOICE%"=="0" exit /b 0
goto MENU

:LAUNCH_GUI
if exist "%FONT%" (
    start "" "%EXEDIR%\%APP%" "%FONT%"
) else (
    start "" "%EXEDIR%\%APP%"
)
goto MENU

:LAUNCH_CONSOLE
start "" "%EXEDIR%\%APP%"
goto MENU

:NO_CMAKE
echo.
echo CMakeが見つかりません。
echo CMakeをインストールするか、Visual StudioのC++開発環境を確認してください。
echo.
pause
exit /b 1

:NO_COMPILER
echo.
echo Visual StudioのC++ビルドツールが見つかりません。
echo Visual Studio Installerで「C++によるデスクトップ開発」を追加してください。
echo.
pause
exit /b 1

:BUILD_ERROR
echo.
echo ビルドに失敗しました。上に表示されたエラーを確認してください。
echo この画面は自動では閉じません。
echo.
pause
exit /b 1
