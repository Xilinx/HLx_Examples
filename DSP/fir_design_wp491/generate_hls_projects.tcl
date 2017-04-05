# *******************************************************************************
# Vendor: Xilinx 
# Associated Filename: generate_hls_projects.tcl
# Purpose: Tcl commands to setup Vivado HLS demo project
# Device: All 
# Revision History: Mar 31, 2017 - initial release
#                                                 
# *******************************************************************************
# Copyright (C) 2013-2017 XILINX, Inc.
# 
# This file contains confidential and proprietary information of Xilinx, Inc. and 
# is protected under U.S. and international copyright and other intellectual 
# property laws.
# 
# DISCLAIMER
# This disclaimer is not a license and does not grant any rights to the materials 
# distributed herewith. Except as otherwise provided in a valid license issued to 
# you by Xilinx, and to the maximum extent permitted by applicable law: 
# (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND WITH ALL FAULTS, AND XILINX 
# HEREBY DISCLAIMS ALL WARRANTIES AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, 
# INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-INFRINGEMENT, OR 
# FITNESS FOR ANY PARTICULAR PURPOSE; and (2) Xilinx shall not be liable (whether 
# in contract or tort, including negligence, or under any other theory of 
# liability) for any loss or damage of any kind or nature related to, arising under 
# or in connection with these materials, including for any direct, or any indirect, 
# special, incidental, or consequential loss or damage (including loss of data, 
# profits, goodwill, or any type of loss or damage suffered as a result of any 
# action brought by a third party) even if such damage or loss was reasonably 
# foreseeable or Xilinx had been advised of the possibility of the same.
# 
# CRITICAL APPLICATIONS
# Xilinx products are not designed or intended to be fail-safe, or for use in any 
# application requiring fail-safe performance, such as life-support or safety 
# devices or systems, Class III medical devices, nuclear facilities, applications 
# related to the deployment of airbags, or any other applications that could lead 
# to death, personal injury, or severe property or environmental damage 
# (individually and collectively, "Critical Applications"). Customer assumes the 
# sole risk and liability of any use of Xilinx products in Critical Applications, 
# subject only to applicable laws and regulations governing limitations on product 
# liability. 
#
# THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS PART OF THIS FILE AT 
# ALL TIMES.

set version "[exec which vivado_hls | sed -e "s:.*\\(20....\\).*:\\1:g" ]"
set system [exec vivado_hls -s]

foreach implementation [list fx fp] {

    open_project -reset proj_period_2.5_${implementation}_FIR_$version
    
    open_solution solution1 ; #or solution_$implementation


    # -unsafe_math_optimizations 
    #		The unsafe_math_optimizations option will ignore the signness of floating point zero and enable associative floating 
    #		point operations so that compiler can do aggressive optimizations on floating point operations. 
    #		The default is off.
    config_compile -unsafe_math_optimizations
    if { $system > "lnx" && $system < "win" } {
        puts "linux system"
        config_compile -name_max_length 250
    } else {
        puts "windows system"
    }

    set_top ${implementation}_FIR
    add_files     FIR.cpp
    add_files     FIR.h
    add_files -tb FIR_test.cpp
    add_files -tb result.golden.dat
    add_files -tb FIR_fp.inc
    add_files -tb FIR_fp_6digits.inc

    #set_part kintex7 ; puts "using a kintex7 part - change if necessary"
    set_part {xcvu9p-flgb2104-2-i-es2} ; puts "using a vu9p-2 part - change if necessary"

    create_clock -period 2.5 -name default
    csim_design -compiler gcc
    csynth_design
#    cosim_design
#    export_design -evaluate verilog
}
