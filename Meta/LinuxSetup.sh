#!/bin/bash
$PWD/Meta/LinuxSetupNoToolchain.sh
if [ ! -d "$PWD/TOOLCHAIN" ]; then
	mkdir TOOLCHAIN
fi
if [ ! -d "$PWD/TOOLCHAIN/GCC-12" ]; then
	$PWD/Meta/BuildGCC.sh
fi
if [ ! -d "$PWD/TOOLCHAIN/CMAKE-3.25.0" ]; then
	$PWD/Meta/BuildCMake.sh
fi
