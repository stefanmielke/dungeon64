#!/bin/bash

sudo rmmod usbserial
sudo rmmod ftdi_sio

sudo ${N64DevKit}/tools/unfloader/UNFLoader -f 3 -c 0 -s 1 -d -r dungeon64.z64