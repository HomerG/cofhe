print ("INFO: Loading Initial Values to the CRYPTOLEQ Hardware")
ccs0301_tasks.write_serial     (ccs0301_header.GPCFG_HOSTIRQ_PAD_CTL,   "0012001A")
ccs0301_tasks.write_serial     (ccs0301_header.GPCFG_UARTSTX_PAD_CTL),  "0010001A";
ccs0301_tasks.write_serial_128 (ccs0301_header.GPCFG_N_ADDR[0],      ccs0301_init_val.fhe_N)
ccs0301_tasks.write_serial_128 (ccs0301_header.GPCFG_NINV_ADDR[0],   ccs0301_init_val.fhe_NINV)
ccs0301_tasks.write_serial     (ccs0301_header.GPCFG_FHECTL_ADDR,    ccs0301_init_val.fhe_ctl)
ccs0301_tasks.write_serial     (ccs0301_header.GPCFG_FHECTL2,        ccs0301_init_val.fhe_ctl2)
ccs0301_tasks.write_serial     (ccs0301_header.GPCFG_FHECTL3,        ccs0301_init_val.fhe_ctl3)
#ccs0301_tasks.write_serial     (ccs0201_header.GPCFG_FHECTLP_ADDR,    "00000200")
