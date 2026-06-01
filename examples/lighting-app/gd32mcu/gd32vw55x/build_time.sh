#!/usr/bin/env bash
# echo "Active CHIP environmnent"
# source third_party/connectedhomeip/scripts/activate.sh

echo -e \
"#ifndef __BUILD_DATE_H__\n"\
"#define __BUILD_DATE_H__\n"\
"\n"\
"/* Do not change the content here, it's auto generated */\n"\
"#define SDK_BUILD_DATE \"`date "+%Y-%m-%d %H:%M:%S"`\"\n"\
"\n"\
"#endif" > ../../../../third_party/gd32mcu/gd32vw55x/GD32VW55x_RELEASE_V1.0.0/MSDK/app/_build_date.h

echo -e \
"#ifndef __BUILD_DATE_H__\n"\
"#define __BUILD_DATE_H__\n"\
"\n"\
"/* Do not change the content here, it's auto generated */\n"\
"#define SDK_BUILD_DATE \"`date "+%Y-%m-%d-%w %H:%M:%S"`\"\n"\
"\n"\
"#endif" > ./include/_build_date_t.h

# ---- Merge MBL + App into a single combined.hex ----
OUTDIR=$1
MBL_HEX="${OUTDIR}/gd32vw55x_mbl.hex"
APP_HEX="${OUTDIR}/chip-gd32vw55x-lighting-example.hex"
COMBINED_HEX="${OUTDIR}/combined.hex"

if [ -f "${MBL_HEX}" ] && [ -f "${APP_HEX}" ]; then
    python3 - <<PYEOF
from intelhex import IntelHex
mbl = IntelHex("${MBL_HEX}")
app = IntelHex("${APP_HEX}")
mbl.merge(app, overlap='replace')
mbl.write_hex_file("${COMBINED_HEX}")
print("Generated: ${COMBINED_HEX}")
PYEOF
else
    echo "Warning: hex files not found, skipping combined.hex generation"
    echo "  Expected: ${MBL_HEX}"
    echo "  Expected: ${APP_HEX}"
fi

