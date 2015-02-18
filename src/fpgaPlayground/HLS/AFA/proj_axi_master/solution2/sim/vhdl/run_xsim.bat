
call xelab xil_defaultlib.apatb_example_top -prj example.prj --lib "ieee_proposed=./ieee_proposed" -s example 
call xsim --noieeewarnings example -tclbatch example.tcl

