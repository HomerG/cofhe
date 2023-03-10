create_clock -period 10.000 -name {pad[2]} [get_ports {pad[2]}]


#Restet Pins
set_property PACKAGE_PIN J15      [get_ports {pad[0]}]
set_property PACKAGE_PIN L16      [get_ports {pad[1]}]
set_property PACKAGE_PIN E3       [get_ports {pad[2]}]  
#UartM
set_property PACKAGE_PIN D4       [get_ports {pad[3]}]
set_property PACKAGE_PIN C4       [get_ports {pad[4]}]
#GPIO
set_property PACKAGE_PIN H17      [get_ports {pad[5]}]
set_property PACKAGE_PIN K15      [get_ports {pad[6]}]
set_property PACKAGE_PIN J13      [get_ports {pad[7]}]
set_property PACKAGE_PIN N14      [get_ports {pad[8]}]
set_property PACKAGE_PIN R18      [get_ports {pad[9]}]
set_property PACKAGE_PIN V17      [get_ports {pad[10]}]

set_property PACKAGE_PIN C17      [get_ports {pad[11]}]
set_property PACKAGE_PIN D18      [get_ports {pad[12]}]
set_property PACKAGE_PIN E18      [get_ports {pad[13]}]
set_property PACKAGE_PIN G17      [get_ports {pad[14]}]
set_property PACKAGE_PIN D17      [get_ports {pad[15]}]
set_property PACKAGE_PIN E17      [get_ports {pad[16]}]
set_property PACKAGE_PIN F18      [get_ports {pad[17]}]
set_property PACKAGE_PIN G18      [get_ports {pad[18]}]
set_property PACKAGE_PIN V11      [get_ports {pad[19]}]
set_property PACKAGE_PIN V15      [get_ports {pad[20]}]
set_property PACKAGE_PIN H4       [get_ports {pad[21]}]
set_property PACKAGE_PIN H1       [get_ports {pad[22]}]
set_property PACKAGE_PIN G1       [get_ports {pad[23]}]
set_property PACKAGE_PIN G3       [get_ports {pad[24]}]
set_property PACKAGE_PIN H2       [get_ports {pad[25]}]

set_property IOSTANDARD  LVCMOS33 [get_ports {pad[0]}]
set_property IOSTANDARD  LVCMOS33 [get_ports {pad[1]}]
set_property IOSTANDARD  LVCMOS33 [get_ports {pad[2]}]
set_property IOSTANDARD  LVCMOS33 [get_ports {pad[3]}]
set_property IOSTANDARD  LVCMOS33 [get_ports {pad[4]}]
set_property IOSTANDARD  LVCMOS33 [get_ports {pad[5]}]
set_property IOSTANDARD  LVCMOS33 [get_ports {pad[6]}]
set_property IOSTANDARD  LVCMOS33 [get_ports {pad[7]}]
set_property IOSTANDARD  LVCMOS33 [get_ports {pad[8]}]
set_property IOSTANDARD  LVCMOS33 [get_ports {pad[9]}]
set_property IOSTANDARD  LVCMOS33 [get_ports {pad[10]}]
set_property IOSTANDARD  LVCMOS33 [get_ports {pad[11]}]
set_property IOSTANDARD  LVCMOS33 [get_ports {pad[12]}]
set_property IOSTANDARD  LVCMOS33 [get_ports {pad[13]}]
set_property IOSTANDARD  LVCMOS33 [get_ports {pad[14]}]
set_property IOSTANDARD  LVCMOS33 [get_ports {pad[15]}]
set_property IOSTANDARD  LVCMOS33 [get_ports {pad[16]}]
set_property IOSTANDARD  LVCMOS33 [get_ports {pad[17]}]
set_property IOSTANDARD  LVCMOS33 [get_ports {pad[18]}]
set_property IOSTANDARD  LVCMOS33 [get_ports {pad[19]}]
set_property IOSTANDARD  LVCMOS33 [get_ports {pad[20]}]
set_property IOSTANDARD  LVCMOS33 [get_ports {pad[21]}]
set_property IOSTANDARD  LVCMOS33 [get_ports {pad[22]}]
set_property IOSTANDARD  LVCMOS33 [get_ports {pad[23]}]
set_property IOSTANDARD  LVCMOS33 [get_ports {pad[24]}]
set_property IOSTANDARD  LVCMOS33 [get_ports {pad[25]}]

set_property DRIVE       8        [get_ports {pad[0]}]
set_property DRIVE       8        [get_ports {pad[1]}]
set_property DRIVE       8        [get_ports {pad[2]}]
set_property DRIVE       8        [get_ports {pad[3]}]
set_property DRIVE       8        [get_ports {pad[4]}]
set_property DRIVE       8        [get_ports {pad[5]}]
set_property DRIVE       8        [get_ports {pad[6]}]
set_property DRIVE       8        [get_ports {pad[7]}]
set_property DRIVE       8        [get_ports {pad[8]}]
set_property DRIVE       8        [get_ports {pad[9]}]
set_property DRIVE       8        [get_ports {pad[10]}]
set_property DRIVE       8        [get_ports {pad[11]}]
set_property DRIVE       8        [get_ports {pad[12]}]
set_property DRIVE       8        [get_ports {pad[13]}]
set_property DRIVE       8        [get_ports {pad[14]}]
set_property DRIVE       8        [get_ports {pad[15]}]
set_property DRIVE       8        [get_ports {pad[16]}]
set_property DRIVE       8        [get_ports {pad[17]}]
set_property DRIVE       8        [get_ports {pad[18]}]
set_property DRIVE       8        [get_ports {pad[19]}]
set_property DRIVE       8        [get_ports {pad[20]}]
set_property DRIVE       8        [get_ports {pad[21]}]
set_property DRIVE       8        [get_ports {pad[22]}]
set_property DRIVE       8        [get_ports {pad[23]}]
set_property DRIVE       8        [get_ports {pad[24]}]
set_property DRIVE       8        [get_ports {pad[25]}]

#set_property ALLOW_COMBINATORIAL_LOOPS TRUE [get_nets u_chip_core_inst/u_random_num_gen*_inst/trng_wrap_inst*/trng_inst[*].u_trng_inst/*]
set_property SEVERITY {Warning} [get_drc_checks LUTLP-1]
