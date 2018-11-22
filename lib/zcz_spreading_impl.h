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

#ifndef INCLUDED_SPREADING_ZCZ_SPREADING_IMPL_H
#define INCLUDED_SPREADING_ZCZ_SPREADING_IMPL_H
#include <iostream>
#include <vector>
#include <spreading/zcz_spreading.h>

namespace gr {
  namespace spreading {

    class zcz_spreading_impl : public zcz_spreading
    {
     private:
      // Nothing to declare in this block.
      std::vector<float> spreading_code;
      int bit_vector_length;
      
     public:
      zcz_spreading_impl(int bit_vector_length, std::vector<float> spreading_code);
      ~zcz_spreading_impl();

      void set_bit_length(int bit_vector_length);
      void set_spread_code(std::vector<float> spreading_code);

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace spreading
} // namespace gr

#endif /* INCLUDED_SPREADING_ZCZ_SPREADING_IMPL_H */


