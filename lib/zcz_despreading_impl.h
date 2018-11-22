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

#ifndef INCLUDED_SPREADING_ZCZ_DESPREADING_IMPL_H
#define INCLUDED_SPREADING_ZCZ_DESPREADING_IMPL_H

#include <spreading/zcz_despreading.h>

namespace gr {
  namespace spreading {

    class zcz_despreading_impl : public zcz_despreading
    {
     private:
      // Nothing to declare in this block.
      int bit_vector_length;
      std::vector<float> despreading_code;
      
      int check1, initial_no_items;
      int no_items, new_sync_pos;
      int check, track;
      
      int d_decim;
      int code_len;
      gr_complex *in_buffer;
      gr_complex *sig;
      
     public:
      zcz_despreading_impl(int bit_vector_length, std::vector<float> despreading_code);
      ~zcz_despreading_impl();

      int set_bit_length(int bit_vector_length);
      int set_bitvector(std::vector<float> despreading_code);

      int skip_data(int noutput_items);
      int synchronization(gr_complex sig[], std::vector<float> code_waveform, int d_len);

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace spreading
} // namespace gr

#endif /* INCLUDED_SPREADING_ZCZ_DESPREADING_IMPL_H */


