@echo off

set CROSS_PREFIX=%1
set MBL_DIR=..\\..\\..
set SCONS_CONFIG=%MBL_DIR%\\..\\MSDK\\macsw\\config\\wlan_config.h
set PLATFORM_CONFIG=..\..\..\..\config\platform_def.h
echo cd=%cd%

:: generate mbl.lds
if "%CROSS_PREFIX%" neq "IAR" (
    %CROSS_PREFIX%gcc -E -P -o ..\mbl.lds -x c-header ..\mbl.ld -I ..\..\..\mainboot -I ..\..\..\..\config
)

:: use different rom_symbol.gcc for FPGA_V7 and FPGA_Ultra and ASIC b_cut
set combo_en=1
findstr /r /c:"#define[ ]*CONFIG_PLATFORM[ ]*PLATFORM_FPGA_32103_V7" %PLATFORM_CONFIG% && set combo_en=0
set asic=1
findstr /r /c:"#define[ ]*CONFIG_PLATFORM[ ]*PLATFORM_FPGA[0-9A-Za-z_]*" %PLATFORM_CONFIG% && set asic=0
set a_cut=0
findstr /r /c:"#define[ ]*ASIC_CUT[ ]*A_CUT" %PLATFORM_CONFIG% && set a_cut=1

if "%asic%" == "1" (
    if "%a_cut%" == "1" (
        echo copy rom_symbol_acut.gcc rom_symbol.gcc
        copy %MBL_DIR%\\..\\ROM-EXPORT\\symbol\\rom_symbol_acut.gcc  %MBL_DIR%\\..\\ROM-EXPORT\\symbol\\rom_symbol.gcc
    ) else if "%a_cut%" == "0" (
        echo copy rom_symbol_bcut.gcc rom_symbol.gcc
        copy %MBL_DIR%\\..\\ROM-EXPORT\\symbol\\rom_symbol_bcut.gcc  %MBL_DIR%\\..\\ROM-EXPORT\\symbol\\rom_symbol.gcc
    )
) else  (
    if "%combo_en%" == "1" (
        echo copy rom_symbol_ultra.gcc rom_symbol.gcc
        copy %MBL_DIR%\\..\\ROM-EXPORT\\symbol\\rom_symbol_ultra.gcc  %MBL_DIR%\\..\\ROM-EXPORT\\symbol\\rom_symbol.gcc
    ) else if "%combo_en%" == "0" (
        echo copy rom_symbol_v7.gcc rom_symbol.gcc
        copy %MBL_DIR%\\..\\ROM-EXPORT\\symbol\\rom_symbol_v7.gcc  %MBL_DIR%\\..\\ROM-EXPORT\\symbol\\rom_symbol.gcc
    )
)
:end
