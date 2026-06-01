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

