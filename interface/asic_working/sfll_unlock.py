#131990 << SFLL KEY VALUE            fffffc20204080010700001eff4c000b
#131990 << FLL  KEY VALUE            9caacea387331b0edbc24b42cd167b8c
print ("INFO: Unlocking Cortex M0 logic locking")
ccs0201_tasks.write_serial_128    (ccs0201_header.GPCFG_SFLL0_ADDR, ccs0201_init_val.sfll_key);
ccs0201_tasks.write_serial_128    (ccs0201_header.GPCFG_FLL0_ADDR,  ccs0201_init_val.fll_key);
