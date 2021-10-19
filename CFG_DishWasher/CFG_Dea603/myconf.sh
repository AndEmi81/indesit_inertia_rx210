#!/bin/bash
cfg_include_file="autoconf.h"
prg_dir="CFG_DishWasher/CFG_Dea700"
mconf pl_config
if [ -e "$cfg_include_file" ]
then
  ls -la "$cfg_include_file"
  mv "$cfg_include_file" "$prg_dir"
  ls -la "$cfg_include_file"
fi
