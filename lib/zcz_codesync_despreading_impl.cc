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
#include "zcz_codesync_despreading_impl.h"

namespace gr {
  namespace spreading {

    zcz_codesync_despreading::sptr
    zcz_codesync_despreading::make(int bitvector_length, std::vector<float> bitvector)
    {
      return gnuradio::get_initial_sptr
        (new zcz_codesync_despreading_impl(bitvector_length, bitvector));
    }

    /*
     * The private constructor
     */
    zcz_codesync_despreading_impl::zcz_codesync_despreading_impl(int bitvector_length, std::vector<float> bitvector)
      : gr::sync_decimator("zcz_codesync_despreading",
              gr::io_signature::make( 1, 1, sizeof(gr_complex)),
              gr::io_signature::make( 1, 1, sizeof(gr_complex)), bitvector_length)
    {
        reg_err[0]=0;
        reg_err[1]=0;
        adj_th = 100;
        d_decim = bitvector_length;
        delta = 0.0;
        delta_stp = 0.25;
        a = 0.8;
        d_decim = bitvector_length;
        in_buffer = new gr_complex[12000];
        sig = new gr_complex[5000];
        data_buffer = new gr_complex[12000];
        
        set_bitvector(bitvector);
        set_bitvector_size(bitvector_length);
    }

    /*
     * Our virtual destructor.
     */
    zcz_codesync_despreading_impl::~zcz_codesync_despreading_impl()
    {
        if(bitvector.size()>0){
            bitvector.clear();
        }
    }
    
    int zcz_codesync_despreading_impl::set_bitvector_size(int _bitvector_length){
        bitvector_length=_bitvector_length;
        return 0;
    }
    
    int zcz_codesync_despreading_impl::set_bitvector(std::vector<float> _bitvector){
        bitvector=_bitvector;
        return 0;
    }
    
    int zcz_codesync_despreading_impl::synchronization(gr_complex sig[], std::vector<float> code_waveform, int d_len){
        int code_size = bitvector_length;
        
        int n=floor(d_len/256.0);
        gr_complex CODE2[code_size*(n-1)];
        int no=0;
        
        for(int i=0;i<n-1; i++){
            for(int j=0;j<code_size;j++){
                CODE2[j+no] = code_waveform.at(j);
            }
            no+=code_size;
        }
        
        float max_corr[256];
        gr_complex temp1[(n-1)*256];
        gr_complex temp2[(n-1)];
        float temp3[n-1];
        float max_c_t[256][n-1];
        
        for(int i=0;i<256;i++){
            int count=0;
            for(int j=i;j<(((n-1)*256)+i);j+=4){
                temp1[count]=sig[j]*(CODE2[count]);
                count++;
            }
            
            gr_complex sum=0;
            int l=0;
            for(int j=0;j<n-1;j++){
                for(int k=0;k<256;k++){
                    sum+=temp1[k+1];
                }
                temp2[j]=sum;
                l+=256;
                sum=0;
            }
            
            for(int j=0;j<n-1;j++){
                temp3[j]=real(temp2[j]*conj(temp2[j]));
                max_c_t[i][j] = temp3[j];
            }
            float sum1=0;
            for(int j=0;j<n-1;j++){
                sum1+=temp3[j];
            }
            max_corr[i] = sum1;
        }
        
        float temp=max_corr[0];
        int pos=0;
        for(int i=1;i<256;i++){
            if(max_corr[i]>temp){
                temp=max_corr[i];
                pos=i;
            }
        }
        
        return pos;
    }
    
    int zcz_codesync_despreading_impl::skip_data(int noutput_items){
        initial_no_items += noutput_items;
        if(initial_no_items < 200000){
            return 0;
        }else{
            check1++;
            return (initial_no_items-200000);
        }
    }
    
    void zcz_codesync_despreading_impl::code_tracking(gr_complex sig_ad[], gr_complex sig_re[], std::vector<float> code){
        gr_complex R1=0,R2=0;
        int count =0;
        for(int i=0;i<256;i+=4){
            R1+=(sig_ad[i]*code.at(count++));
            R2+=(sig_re[i]*code.at(count++));
        }
        float err_in=abs(R1)-abs(R2);
        // IIR loop filtering
        reg_err[0]=reg_err[1];
        reg_err[1]=a*reg_err[0]+(1-a)*err_in;
        
        if(reg_err[1]>=0){
            fast_cnt++;//early branch win
        }else{
            late_cnt++;//late branch win
        }
        
        if(fast_cnt>=adj_th){
            delta+=delta_stp;
            fast_cnt=0;
            late_cnt=0;//makes incoming signal faster
        }
        if(late_cnt>=adj_th){
            delta-=delta_stp;
            fast_cnt=0;
            late_cnt=0;
        }
    }
    
    int
    zcz_codesync_despreading_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      gr_complex *in = (gr_complex *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];

      gr_complex sum1 = 0;
      
      int co=0;
      int offset;
      
      if( ((offset=skip_data(noutput_items*d_decim))==0) && check1==0){
        //skip initial data
      }else if( no_items < 8000+240 ){
        if(check1){
            check1++;
            for(int j=(noutput_items*d_decim)-offset;j<noutput_items*d_decim; j++){
                in_buffer[no_items++] = in[j];
            }
        }else{
            for(int i=no_items; i<(noutput_items*d_decim)+no_items; i++){
                in_buffer[i] = in[co++];
                no_items+=noutput_items*d_decim;
            }
        }
      }else{
        if(check==0){
            int OverSampRation=4;
            int L=64;
            int data_4_sync=32;
            gr_complex sig_4sync[64*4*(32-2)];
            
            int tempc=0;
            for(int i=240;i<64*4*30+240; i++){
                sig_4sync[tempc++]=in_buffer[i];
            }
            
            int sync_pos=32;
            int d_len=64*4*(32-2);
            
            sync_pos = synchronization(sig_4sync, bitvector, bitvector_length);
            
            check++;
            new_sync_pos=sync_pos+240;
            
            while(1){
                new_sync_pos+=256;
                if(new_sync_pos>=no_items){
                    new_sync_pos-=256;
                    break;
                }
            }
            new_sync_pos-=1;
            track = no_items-new_sync_pos;
            memcpy(sig, in_buffer+(new_sync_pos), (no_items-new_sync_pos)*sizeof(gr_complex));
        }else{
            memcpy(sig+track,in,(noutput_items*d_decim)*sizeof(gr_complex));
            gr_complex temp_out[5000];
            
            int coun=0,count=0,count1n=0,cnt256=1,Plen=512,cnt4PSYNC=0,count_out=0,cou=0;
            gr_complex reg264[264],din,s_cur=0,s_pre=0,s_cur_pre=0,rdata_NoCFO[1000],cfo_est,temp1=0;
            
            for(int tt=0; tt < noutput_items*d_decim; tt++){
                din = sig[tt];
                
                for(int k=0;k<263;k++){
                    reg264[k]=reg264[k+1];
                }
                reg264[263]=din;// new data at the endif
                
                if(cnt256==256){
                    int co=0;
                    //early branch
                    for(int j=8;j<264;j++){
                        sig_ad[co++] = reg264[j];
                    }
                    co=0;
                    //correct data buffer which will be sent to despreading
                    for(int k=4;k<260;k++){
                        sig_mid[co++]=reg264[k];
                    }
                    //late branch
                    for(int j=0;j<256;j++){
                        sig_re[co++]=reg264[j];
                    }
                    //delta controls the movement of cnt256
                    if(delta>=1){
                        delta=0;
                        cnt_1stop=1;
                        cnt_plus1=0;
                    }
                    if(delta<=-1){
                        delta = 0;
                        cnt_plus1 = 1;
                        cnt_1stop = 0;
                    }
                    
                    code_tracking(sig_ad, sig_re, bitvector);
                    
                    cnt256 = 0;
                    for(int j=0; j<d_decim; j+=4){
                        //despreading carried out at sig_mid
                        temp1+=(sig_mid[j])*(code[count++]);
                    }
                    out[count_out++]=temp1;
                    coun+=d_decim;
                    count=0;
                    temp1=0;
                }
                
                if(cnt4<=3){
                    cnt4++;
                }
                
                if(cnt4==4){
                    if(cnt_1stop<1){
                        if(cnt_plus1<1){
                            cnt256++;
                        }else{
                            cnt256+=2;
                            cnt_plus1=0;
                        }
                    }else{
                        cnt_1stop=0;
                    }
                }
            }
        }
      }

      // Do <+signal processing+>
      // Move remaining data in in_buffer from new sync position to sig
      for(int i=0;i<track;i++){
        sig[i] = sig[(noutput_items*d_decim)+1];
      }

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }
































  } /* namespace spreading */
} /* namespace gr */


