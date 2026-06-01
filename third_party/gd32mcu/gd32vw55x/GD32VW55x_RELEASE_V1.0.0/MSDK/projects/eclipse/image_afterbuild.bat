::     \file    image_afterbuild.bat
::     \brief   Bat file for image after build.
::
::     \version 2023-07-20, V1.0.0, firmware for GD32VW55x
::
::     Copyright (c) 2023, GigaDevice Semiconductor Inc.
::
::     Redistribution and use in source and binary forms, with or without modification,
:: are permitted provided that the following conditions are met:
::
::     1. Redistributions of source code must retain the above copyright notice, this
::        list of conditions and the following disclaimer.
::     2. Redistributions in binary form must reproduce the above copyright notice,
::        this list of conditions and the following disclaimer in the documentation
::        and/or other materials provided with the distribution.
::     3. Neither the name of the copyright holder nor the names of its contributors
::        may be used to endorse or promote products derived from this software without
::        specific prior written permission.
::
::     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
:: AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
:: WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
:: IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
:: INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
:: NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
:: PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
:: WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
:: ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
:: OF SUCH DAMAGE.

@echo off
set TOOLKIT=%1
set ALGO_SIGN=%2
set WITH_CERT=%3
set OPENOCD_PATH=%4
set AESK=%5

set ALGO_HASH=SHA256
::set ALGO_SIGN=ECDSA256
set ROOT=..\\..\\..\\..\\..\\
::set OPENOCD_PATH=D:\\tools\\eclipse_tool\\OpenOCD_2022.4.6
set TARGET=msdk

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
echo %AESK%

set MBL_KEY=%ROOT%\scripts\certs\%ALGO_SIGN%\mbl-key.pem
set ROTPK=%ROOT%\scripts\certs\%ALGO_SIGN%\rot-key.pem
set MBL_CERT=%ROOT%\scripts\certs\%ALGO_SIGN%\mbl-cert.pem
set CONFIG_FILE=..\..\config\config_gdm32.h
set IMGTOOL=%ROOT%\scripts\imgtool\imgtool.py
set HEXTOOL=%ROOT%\scripts\imgtool\hextool.py
set GENTOOL=%ROOT%\scripts\imgtool\gentool.py
set AESTOOL=%ROOT%\scripts\imgtool\aestool.py
set SREC_CAT=%ROOT%\scripts\imgtool\srec_cat.exe
set OUTPUT_PATH=%ROOT%\\scripts\\images\\
set DOWNLOAD_BIN=%OUTPUT_PATH%\%TARGET%-sign%AES_SUFFIX%.bin

echo "%TARGET%.elf"
%TOOLKIT%objcopy -O binary -j ".log" "%TARGET%.elf" "trace.bin"
:: %TOOLKIT%objcopy -R ".log" "%TARGET%.elf" "%TARGET%.elf"


if "%TOOLKIT%" neq "IAR" (
    %TOOLKIT%objdump -S -l -d %TARGET%.elf > %TARGET%.dump
    %TOOLKIT%objcopy -O binary --remove-section ".log" %TARGET%.elf %TARGET%.bin
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
:: Check if need python to add sysset\mbl_header\mbl_tailer
:: if mbl_offset is equal to 0, which means boot from MBL directly (not from ROM)
if "%mbl_offset%" == "0x0"  (
    echo "Not add image header and tailer, goto download!"
    copy %TARGET%.bin %OUTPUT_PATH%\\%TARGET%.bin
    set DOWNLOAD_BIN=%TARGET%.bin

    %SREC_CAT% %OUTPUT_PATH%\mbl.bin -Binary -offset "0" ^
                 %OUTPUT_PATH%\%TARGET%.bin -Binary -offset "0xA000" ^
                 -o %OUTPUT_PATH%\image-all.bin -Binary
    goto download
)

REM if exist %OUTPUT_PATH%\%TARGET%-sign.bin del %OUTPUT_PATH%\%TARGET%-sign.bin

if '%WITH_CERT%' == 'CERT' (
    python %IMGTOOL% sign --config %CONFIG_FILE% ^
                      -k %MBL_KEY% ^
                      %KEY_PASSPHRASE% ^
                      -t "IMG" ^
                      --algo_hash "%ALGO_HASH%" ^
                      --algo_sig "%ALGO_SIGN%" ^
                      --cert %MBL_CERT% ^
                      --cert_key %ROTPK% ^
                      %TARGET%.bin %OUTPUT_PATH%\%TARGET%-sign.bin
) else (
    python %IMGTOOL% sign --config %CONFIG_FILE% ^
                      -k %ROTPK% ^
                      %KEY_PASSPHRASE% ^
                      -t "IMG" ^
                      --algo_hash "%ALGO_HASH%" ^
                      --algo_sig "%ALGO_SIGN%" ^
                      %TARGET%.bin %OUTPUT_PATH%\%TARGET%-sign.bin
)

if "%AESK%" == ""  (
    python %HEXTOOL% -c %CONFIG_FILE% ^
            -t "IMG_0" ^
            -e %SREC_CAT% ^
            %OUTPUT_PATH%\%TARGET%-sign.bin ^
            %OUTPUT_PATH%\%TARGET%-sign.hex
)  else (
    python %AESTOOL% --c %CONFIG_FILE%   ^
            -t "IMG_0" ^
            -i %OUTPUT_PATH%\%TARGET%-sign.bin ^
            -o %OUTPUT_PATH%\%TARGET%-sign%AES_SUFFIX%.bin ^
            -k %AESK%
    echo Encrypted!
)

python %GENTOOL% --config %CONFIG_FILE% ^
                 --sys_set %OUTPUT_PATH%\mbl-sys%AES_SUFFIX%.bin ^
                 --img_0 %OUTPUT_PATH%\%TARGET%-sign%AES_SUFFIX%.bin ^
                 -o ""%OUTPUT_PATH%\image-all-sign.bin""

:download
set OPENOCD="%OPENOCD_PATH%\\openocd.exe"
set LINKCFG="%OPENOCD_PATH%\\openocd_gdlink_gd32103.cfg"
::set LINKCFG="%OPENOCD_PATH%\\openocd_gdlink_gd32103_jlink.cfg"
@echo on
::%OPENOCD% -f %LINKCFG% -c "program %DOWNLOAD_BIN% 0x0800A000 verify reset exit;"
:end
