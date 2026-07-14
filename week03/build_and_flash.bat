@echo off
setlocal enabledelayedexpansion

:: ============================================================================
:: HW03: AUTOMATION SCRIPT FOR BUILD AND FLASH STM32 FIRMWARE
:: Target MCU : STM32F103C8T6 (Blue Pill) | Flash Address: 0x08000000
:: ============================================================================

echo STEP 1: CLEANING BUILD DIRECTORY
if exist "build" (
    echo Deleting existing build folder...
    rmdir /s /q "build"
    if exist "build" (
        echo [ERROR] Failed to delete build directory. Close open files inside it.
        goto :error_exit
    )
) else (
    echo Build folder does not exist. Proceeding...
)

echo.
echo STEP 2: CONFIGURING PROJECT WITH CMAKE
cmake -B build -G "Ninja"
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] CMake configuration failed!
    goto :error_exit
)

echo.
echo STEP 3: COMPILING FIRMWARE WITH NINJA
ninja -C build
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Compilation failed! Please check your C code syntax.
    goto :error_exit
)

if not exist "build\app_firmware.bin" (
    echo [ERROR] Output binary build\app_firmware.bin was not generated!
    goto :error_exit
)
echo Generating binary flash image: build/app_firmware.bin

echo.
echo STEP 4: FLASHING FIRMWARE TO TARGET MCU
STM32_Programmer_CLI -c port=SWD -w build\app_firmware.bin 0x08000000 -v -rst
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Flashing failed! Check ST-Link USB and SWD wiring (SWDIO/SWCLK/GND).
    goto :error_exit
)

echo.
echo ============================================================================
echo SUCCESS: BUILD AND FLASH COMPLETED!
echo ============================================================================
exit /b 0

:error_exit
echo.
echo ============================================================================
echo FAILED: PROCESS ABORTED DUE TO ERRORS.
echo ============================================================================
exit /b 1