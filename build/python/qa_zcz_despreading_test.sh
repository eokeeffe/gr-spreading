#!/bin/sh
export VOLK_GENERIC=1
export GR_DONT_LOAD_PREFS=1
export srcdir=/home/evan/SignalProcessing/GNURadio-Modules/gr-spreading/python
export PATH=/home/evan/SignalProcessing/GNURadio-Modules/gr-spreading/build/python:$PATH
export LD_LIBRARY_PATH=/home/evan/SignalProcessing/GNURadio-Modules/gr-spreading/build/lib:$LD_LIBRARY_PATH
export PYTHONPATH=/home/evan/SignalProcessing/GNURadio-Modules/gr-spreading/build/swig:$PYTHONPATH
/usr/bin/python2 /home/evan/SignalProcessing/GNURadio-Modules/gr-spreading/python/qa_zcz_despreading.py 
