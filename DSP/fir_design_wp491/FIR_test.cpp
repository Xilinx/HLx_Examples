/*******************************************************************************
Vendor: Xilinx 
Associated Filename: FIR_test.cpp
Purpose:Vivado HLS Coding Style example 
Device: All 
Revision History: May 30, 2008 - initial release
                                                
*******************************************************************************
#-  (c) Copyright 2011-2016 Xilinx, Inc. All rights reserved.
#-
#-  This file contains confidential and proprietary information
#-  of Xilinx, Inc. and is protected under U.S. and
#-  international copyright and other intellectual property
#-  laws.
#-
#-  DISCLAIMER
#-  This disclaimer is not a license and does not grant any
#-  rights to the materials distributed herewith. Except as
#-  otherwise provided in a valid license issued to you by
#-  Xilinx, and to the maximum extent permitted by applicable
#-  law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND
#-  WITH ALL FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES
#-  AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
#-  BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-
#-  INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE; and
#-  (2) Xilinx shall not be liable (whether in contract or tort,
#-  including negligence, or under any other theory of
#-  liability) for any loss or damage of any kind or nature
#-  related to, arising under or in connection with these
#-  materials, including for any direct, or any indirect,
#-  special, incidental, or consequential loss or damage
#-  (including loss of data, profits, goodwill, or any type of
#-  loss or damage suffered as a result of any action brought
#-  by a third party) even if such damage or loss was
#-  reasonably foreseeable or Xilinx had been advised of the
#-  possibility of the same.
#-
#-  CRITICAL APPLICATIONS
#-  Xilinx products are not designed or intended to be fail-
#-  safe, or for use in any application requiring fail-safe
#-  performance, such as life-support or safety devices or
#-  systems, Class III medical devices, nuclear facilities,
#-  applications related to the deployment of airbags, or any
#-  other applications that could lead to death, personal
#-  injury, or severe property or environmental damage
#-  (individually and collectively, "Critical
#-  Applications"). Customer assumes the sole risk and
#-  liability of any use of Xilinx products in Critical
#-  Applications, subject only to applicable laws and
#-  regulations governing limitations on product liability.
#-
#-  THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS
#-  PART OF THIS FILE AT ALL TIMES. 
#- ************************************************************************


This file contains confidential and proprietary information of Xilinx, Inc. and 
is protected under U.S. and international copyright and other intellectual 
property laws.

DISCLAIMER
This disclaimer is not a license and does not grant any rights to the materials 
distributed herewith. Except as otherwise provided in a valid license issued to 
you by Xilinx, and to the maximum extent permitted by applicable law: 
(1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND WITH ALL FAULTS, AND XILINX 
HEREBY DISCLAIMS ALL WARRANTIES AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, 
INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-INFRINGEMENT, OR 
FITNESS FOR ANY PARTICULAR PURPOSE; and (2) Xilinx shall not be liable (whether 
in contract or tort, including negligence, or under any other theory of 
liability) for any loss or damage of any kind or nature related to, arising under 
or in connection with these materials, including for any direct, or any indirect, 
special, incidental, or consequential loss or damage (including loss of data, 
profits, goodwill, or any type of loss or damage suffered as a result of any 
action brought by a third party) even if such damage or loss was reasonably 
foreseeable or Xilinx had been advised of the possibility of the same.

CRITICAL APPLICATIONS
Xilinx products are not designed or intended to be fail-safe, or for use in any 
application requiring fail-safe performance, such as life-support or safety 
devices or systems, Class III medical devices, nuclear facilities, applications 
related to the deployment of airbags, or any other applications that could lead 
to death, personal injury, or severe property or environmental damage 
(individually and collectively, "Critical Applications"). Customer asresultes the 
sole risk and liability of any use of Xilinx products in Critical Applications, 
subject only to applicable laws and regulations governing limitations on product 
liability. 

THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS PART OF THIS FILE AT 
ALL TIMES.

*******************************************************************************/
#include "FIR.h"

int main() {
    std::ifstream gold_ifs;
    int outside_range=0;

    gold_ifs.open("result.golden.dat");
    bool eof=false,use_golden=true;
    double input;
    const double errorpc=1;
    const int total_test_values=10000;

    // Apply stimuli, call the top-level function and save the results
    for (int i = 0; i < total_test_values; i++) {
        int gold_intref;
        if(!eof) {
            int index_unused;
            gold_ifs >> index_unused >> gold_intref;
            eof = gold_ifs.eof();
        }
        if (use_golden) {
            input=i;
        } else {
            input = 100. * (1.+sin(i/10.)) - 50. * cos(i/15.);
        }
        //precision of input set to 5 digits
        input=double(int(input*1024*16))/1024/16;
        
        /** DUTs: one is reference one is DUT **/
        fp_acc_t fp_output = fp_FIR(input);
        fx_acc_t fx_output = fx_FIR(input);
        
        if (use_golden) {
            double gold_ref = (double)gold_intref / (1<<15);
            float fp_error_percent = i>0 ? (fp_output-gold_ref)/gold_ref*100 : 0;
            float fx_error_percent = i>0 ? (fx_output.to_double()-gold_ref)/gold_ref*100 : 0;

            if(fx_error_percent>errorpc || fx_error_percent<-errorpc || fp_error_percent>errorpc || fp_error_percent<-errorpc) {
            std::cout << " idx:" << std::setw( 5) << i;
            std::cout << " input"<< std::setw(10) << input;
            std::cout << " ref:" << std::setw(10) << gold_ref;
            std::cout << " fp:"  << std::setw(10) << fp_output;
            std::cout << " e%:"  << std::setw( 3) << fp_error_percent;
            std::cout << " fx:"  << std::setw(10) << fx_output;
            std::cout << " e%:"  << std::setw( 3) << fx_error_percent;
                std::cout << "  <--- outside "<<errorpc<<"%";
                std::cout << std::endl;
                outside_range++;
            }
        } else {
            float fp_fx_error_percent = i>0 ? (fx_output.to_double()-fp_output)/fp_output*100 : 0;
            if(fp_fx_error_percent>errorpc || fp_fx_error_percent<-errorpc) {
            std::cout << " idx:" << std::setw( 5) << i;
            std::cout << " input"<< std::setw(10) << input;
            std::cout << " fp:"  << std::setw(10) << fp_output;
            std::cout << " fx:"  << std::setw(10) << fx_output;
            std::cout << " e%:"  << std::setw( 3) << fp_fx_error_percent;
                std::cout << "  <--- outside "<<errorpc<<"%";
                std::cout << std::endl;
                outside_range++;
            }
        }

        use_golden = !eof;

    }
    gold_ifs.close();

    std::cout << "* "<<total_test_values<<" values tested, "<<outside_range<<" values outside an arbitrary range of "<<errorpc<<"%"<<std::endl;
    std::cout << "* overall error rate "<<(double)outside_range/total_test_values<<std::endl;
    return 0;
}

// XSIP watermark, do not delete 67d7842dbbe25473c3c32b93c0da8047785f30d78e8a024de1b57352245f9689
