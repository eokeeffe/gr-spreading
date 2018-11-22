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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "zcz_spreading_impl.h"

namespace gr {
  namespace spreading {

    zcz_spreading::sptr zcz_spreading::make(int bit_vector_length,std::vector<float> spreading_code)
    {
      return gnuradio::get_initial_sptr
        (new zcz_spreading_impl(bit_vector_length,spreading_code));
    }

    /*
     * The private constructor
     */
    zcz_spreading_impl::zcz_spreading_impl(int bit_vector_length,std::vector<float> spreading_code)
      : gr::sync_interpolator("zcz_spreading",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(gr_complex)), bit_vector_length)
    {
        set_bit_length(bit_vector_length);
        set_spread_code(spreading_code);
    }

    /*
     * Our virtual destructor.
     */
    zcz_spreading_impl::~zcz_spreading_impl()
    {
        this->spreading_code.clear();
    }

    void zcz_spreading_impl::set_bit_length(int bit_vector_length){
        this->bit_vector_length = bit_vector_length;
    }
    
    void zcz_spreading_impl::set_spread_code(std::vector<float> spreading_code){
        this->spreading_code = spreading_code;
    }

    int
    zcz_spreading_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      gr_complex *in = (gr_complex *)input_items[0];
      gr_complex *out = (gr_complex *)output_items[0];

      // Do <+signal processing+>
      int no=0;
      for(int i=0;i<noutput_items/bit_vector_length;i++){
        for(int j=0;j<bit_vector_length;j++){
            out[j+no] = in[i] * spreading_code.at(j);
        }
        no+=bit_vector_length;
      }
      no-=bit_vector_length;
      
      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace spreading */
} /* namespace gr */


