mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir := $(dir $(mkfile_path))
USER_LIB_PATH=$(mkfile_dir)
ISP_PROG=avrispmkii
include /usr/share/arduino/Arduino.mk
