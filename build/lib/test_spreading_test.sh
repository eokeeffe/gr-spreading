#!/bin/sh
export VOLK_GENERIC=1
export GR_DONT_LOAD_PREFS=1
export srcdir=/home/evan/SignalProcessing/GNURadio-Modules/gr-spreading/lib
export PATH=/home/evan/SignalProcessing/GNURadio-Modules/gr-spreading/build/lib:$PATH
export LD_LIBRARY_PATH=/home/evan/SignalProcessing/GNURadio-Modules/gr-spreading/build/lib:$LD_LIBRARY_PATH
export PYTHONPATH=$PYTHONPATH
test-spreading 
