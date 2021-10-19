#!/bin/bash
cd ..
cfg_include_file="autoconf.h"
prg_dir="CFG_DishWasher/CFG_Inertia"


if which mconf 2>/dev/null
then
  echo "PLConfig OK"
else
  export PATH=$PATH:./PLConfig
fi
if [ -e $prg_dir/".config" ]
then
  	cp $prg_dir/".config" ".config"
fi
export KCONFIG_CONFIG=$PWD/.config
mconf PLConfig/pl_config
if [ -e "$cfg_include_file" ]
then
  ls -la "$cfg_include_file"
  mv "$cfg_include_file" "$prg_dir"
  ls -la "$prg_dir"/"$cfg_include_file"
fi

if [ -e $prg_dir/".config" ]
then
  	mv $prg_dir/".config" $prg_dir/".config.old"
fi
if [ -e "auto.conf" ]
then
  	rm "auto.conf"
fi
if [ -e ".config" ]
then
	cp -f ".config" $prg_dir
  	rm ".config"
fi
if [ -e ".config.old" ]
then
	cp -f ".config.old" $prg_dir
  	rm ".config.old"
fi
