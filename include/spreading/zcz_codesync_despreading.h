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


#ifndef INCLUDED_SPREADING_ZCZ_CODESYNC_DESPREADING_H
#define INCLUDED_SPREADING_ZCZ_CODESYNC_DESPREADING_H

#include <spreading/api.h>
#include <gnuradio/sync_decimator.h>
#include <vector>

namespace gr {
  namespace spreading {

    /*!
     * \brief <+description of block+>
     * \ingroup spreading
     *
     */
    class SPREADING_API zcz_codesync_despreading : virtual public gr::sync_decimator
    {
     public:
      typedef boost::shared_ptr<zcz_codesync_despreading> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of spreading::zcz_codesync_despreading.
       *
       * To avoid accidental use of raw pointers, spreading::zcz_codesync_despreading's
       * constructor is in a private implementation
       * class. spreading::zcz_codesync_despreading::make is the public interface for
       * creating new instances.
       */
      //static sptr make();
      static sptr make(int bit_vector_length, std::vector<float> user_code); 
    };

  } // namespace spreading
} // namespace gr

#endif /* INCLUDED_SPREADING_ZCZ_CODESYNC_DESPREADING_H */

