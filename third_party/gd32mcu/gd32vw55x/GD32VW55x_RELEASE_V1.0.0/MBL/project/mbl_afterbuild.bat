@echo off
set TOOLKIT=%1
set ALGO_SIGN=%2
set WITH_CERT=%3
set OPENOCD_PATH=%4
set AESK=%~5

set ALGO_HASH=SHA256
set ROOT=..\\..\\..\\..\\
set TARGET=mbl


::set TARGET=eclipse\\Debug\\mbl
::set ALGO_SIGN=ECDSA256
::set ROOT=..\\..\\
::set OPENOCD_PATH=D:\\tools\\RiscV\\OpenOCD_2022.4.6
::set AESK=112233445566778899aabbccddeeff00
::echo ROOT=%ROOT%

if not '%ALGO_SIGN%' == 'ECDSA256' if not '%ALGO_SIGN%' == 'ED25519' (
    echo ALGO_SIGN must be 'ECDSA256' or 'ED25519'!
    goto end
)

if '%ALGO_SIGN%' == 'ED25519' (
    set KEY_PASSPHRASE=-P "12345678"
) else (
    set KEY_PASSPHRASE=
)

if "%AESK%" NEQ "" (
    set AES_SUFFIX=-aes
) else (
    set AES_SUFFIX=
)


set MBL_KEY=%ROOT%\scripts\certs\%ALGO_SIGN%\mbl-key.pem
set ROTPK=%ROOT%\scripts\certs\%ALGO_SIGN%\rot-key.pem
set MBL_CERT=%ROOT%\scripts\certs\%ALGO_SIGN%\mbl-cert.pem
set CONFIG_FILE=..\..\config\config_gdm32.h
set SYSTOOL=%ROOT%\scripts\imgtool\sysset.py
set IMGTOOL=%ROOT%\scripts\imgtool\imgtool.py
set HEXTOOL=%ROOT%\scripts\imgtool\hextool.py
set GENTOOL=%ROOT%\scripts\imgtool\gentool.py
set AESTOOL=%ROOT%\scripts\imgtool\aestool.py
set SREC_CAT=%ROOT%\scripts\imgtool\srec_cat.exe
set OUTPUT_PATH=%ROOT%\\scripts\\images\\
set DOWNLOAD_BIN=%OUTPUT_PATH%\mbl-sys%AES_SUFFIX%.bin

:: echo pwd = "%CD%"

:: Generate dump and bin file
if "%TOOLKIT%" neq "IAR" (
    %TOOLKIT%objdump -d %TARGET%.elf >  %TARGET%.dump
    %TOOLKIT%objcopy -O binary %TARGET%.elf  %TARGET%.bin
)

IF EXIST %OUTPUT_PATH%\%TARGET%.bin  del %OUTPUT_PATH%\%TARGET%.bin
IF EXIST %DOWNLOAD_BIN% del %DOWNLOAD_BIN%

:: find RE_MBL_OFFSET defined in CONFIG_FILE
set mbl_offset=0x0
for /f "tokens=1,2,3" %%i in ( %ROOT%\config\config_gdm32.h ) do (
    if "%%j" == "RE_MBL_OFFSET" (
        set mbl_offset=%%k
    )
)
:: Check if need python to add sysset\mbl header\mbl tailer
if "%mbl_offset%" == "0x0"  (
    echo "Not add image header and tailer, goto download!"
	copy %TARGET%.bin %OUTPUT_PATH%\%TARGET%.bin
	set DOWNLOAD_BIN=%TARGET%.bin
    goto download
)

:: Print ROTPK HASH
::python %IMGTOOL% getpub -k %ROTPK%  %KEY_PASSPHRASE%  --sha256 1

:: Generate system setting hex
python %SYSTOOL% -t "SYS_SET" -c %CONFIG_FILE% %OUTPUT_PATH%\sysset.bin


:: Generate system status hex (padding with 0xFF)
:: python %SYSTOOL% -t "SYS_STATUS" -c %CONFIG_FILE%  %OUTPUT_PATH%\sysstatus.bin

IF EXIST %OUTPUT_PATH%\mbl-sign.bin  del %OUTPUT_PATH%\mbl-sign.bin

:: Add image header, ptlvs and concatenate the cert

if '%WITH_CERT%' == 'CERT' (
    python %IMGTOOL% sign --config %CONFIG_FILE% ^
                      -k %MBL_KEY% ^
                      %KEY_PASSPHRASE% ^
                      -t "MBL" ^
                      --algo_hash "%ALGO_HASH%" ^
                      --algo_sig "%ALGO_SIGN%" ^
                      --cert %MBL_CERT% ^
                      --cert_key %ROTPK% ^
                      %TARGET%.bin %OUTPUT_PATH%\mbl-sign.bin
) else (
    python %IMGTOOL% sign --config %CONFIG_FILE% ^
                      -k %ROTPK% ^
                      %KEY_PASSPHRASE% ^
                      -t "MBL" ^
                      --algo_hash "%ALGO_HASH%" ^
                      --algo_sig "%ALGO_SIGN%" ^
                      %TARGET%.bin %OUTPUT_PATH%\mbl-sign.bin
)

python %GENTOOL% --config %CONFIG_FILE% ^
                 --sys_set %OUTPUT_PATH%\sysset.bin ^
                 --mbl %OUTPUT_PATH%\mbl-sign.bin ^
                 -o %OUTPUT_PATH%\mbl-sys.bin
IF EXIST %OUTPUT_PATH%\sysset.bin del %OUTPUT_PATH%\sysset.bin
IF EXIST %OUTPUT_PATH%\mbl-sign.bin del %OUTPUT_PATH%\mbl-sign.bin

if "%AESK%" == ""  (
    python %HEXTOOL% -c %CONFIG_FILE% ^
            -t "SYS_SET" ^
            -e %SREC_CAT% ^
            %OUTPUT_PATH%\mbl-sys.bin ^
            %OUTPUT_PATH%\mbl-sys.hex
)  else (
    python %IMGTOOL% pad -s 0x8000 ^
                         %OUTPUT_PATH%\mbl-sys.bin %OUTPUT_PATH%\mbl-sys-pad.bin
    python %AESTOOL% --c %CONFIG_FILE%   ^
            -t "SYS_SET" ^
            -i %OUTPUT_PATH%\mbl-sys-pad.bin ^
            -o %OUTPUT_PATH%\mbl-sys%AES_SUFFIX%.bin ^
            -k %AESK%
    python %HEXTOOL% -c %CONFIG_FILE% ^
            -t "SYS_SET" ^
            -e %SREC_CAT% ^
            %OUTPUT_PATH%\mbl-sys%AES_SUFFIX%.bin ^
            %OUTPUT_PATH%\mbl-sys.hex
    del %OUTPUT_PATH%\mbl-sys-pad.bin
    echo Encrypted!
)

:download
set OPENOCD="%OPENOCD_PATH%\\openocd.exe"
set LINKCFG="%OPENOCD_PATH%\\openocd_gdlink_gd32103.cfg"
::set LINKCFG="%OPENOCD_PATH%\\openocd_gdlink_gd32103_jlink.cfg"
@echo on
::%OPENOCD% -f %LINKCFG% -c "program %DOWNLOAD_BIN% 0x08000000 verify exit;"
:end
