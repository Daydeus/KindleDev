#!/bin/bash
# Name: KindleDev
# Author: Daydeus
# DontUseFBInk
# Last-opened: 0

timestamp=$(date +%s)
sed -i "s/^# Last-opened:.*/# Last-opened: $timestamp/" "$0" &

/mnt/us/extensions/kindleDev/kindleDev