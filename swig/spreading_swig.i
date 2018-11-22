/* -*- c++ -*- */

#define SPREADING_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "spreading_swig_doc.i"

%{
#include "spreading/zcz_spreading.h"
#include "spreading/zcz_despreading.h"
#include "spreading/zcz_codesync_despreading.h"
%}


%include "spreading/zcz_spreading.h"
GR_SWIG_BLOCK_MAGIC2(spreading, zcz_spreading);
%include "spreading/zcz_despreading.h"
GR_SWIG_BLOCK_MAGIC2(spreading, zcz_despreading);
%include "spreading/zcz_codesync_despreading.h"
GR_SWIG_BLOCK_MAGIC2(spreading, zcz_codesync_despreading);
