/* -*- c++ -*- */
/* 
 * Copyright 2018 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_SPREADING_ZCZ_CODESYNC_DESPREADING_IMPL_H
#define INCLUDED_SPREADING_ZCZ_CODESYNC_DESPREADING_IMPL_H

#include <spreading/zcz_codesync_despreading.h>
#include <math.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <vector>

namespace gr {
  namespace spreading {

    class zcz_codesync_despreading_impl : public zcz_codesync_despreading
    {
     private:
      // Nothing to declare in this block.
      std::vector<float> bitvector;
      int bitvector_length;
      
      int check,c,track,check1,fast_cnt,
          late_cnt,flag1,new_sync_pos,
          initial_no_items,no_items, 
          cnt_1stop,cnt_plus1,cnt4,adj_th;
      gr_complex sig_ad[256], sig_mid[256], sig_re[256];
      float delta,delta_stp,a,code[64],reg_err[2];
      
      int d_decim;
      gr_complex *in_buffer;
      gr_complex *sig;
      gr_complex *data_buffer;
      
     public:
      zcz_codesync_despreading_impl(int bitvector_length, std::vector<float> bitvector);
      ~zcz_codesync_despreading_impl();

      int set_bitvector_size(int bitvector_length);
      int set_bitvector(std::vector<float> bitvector);
      
      int synchronization(gr_complex sig[], std::vector<float> code_waveform, int d_len);
      int skip_data(int noutput_items);
      void code_tracking(gr_complex sig_ad[], gr_complex sig_re[], std::vector<float> code);
      
      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace spreading
} // namespace gr

#endif /* INCLUDED_SPREADING_ZCZ_CODESYNC_DESPREADING_IMPL_H */


