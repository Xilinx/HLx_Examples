ARCHNAME=archive_`date +%Y%m%d`_`whoami`.zip
zip $ARCHNAME archive_me.bash FIR.cpp FIR_fp_6digits.inc FIR_fp.inc FIR.h FIR_test.cpp generate_hls_projects.tcl result.golden.dat
ls -rtl -h $ARCHNAME
unzip -t $ARCHNAME
