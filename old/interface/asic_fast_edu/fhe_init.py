print ("INFO: Loading Initial Values to the FHE Hardware")
ccs0302_tasks_fast.write_serial     (ccs0302_header.GPCFG_HOSTIRQ_PAD_CTL,  "0012001A")
ccs0302_tasks_fast.write_serial     (ccs0302_header.GPCFG_UARTSTX_PAD_CTL,  "0010001A")
ccs0302_tasks_fast.write_serial_128 (ccs0302_header.GPCFG_N_ADDR[0],        ccs0302_init_val.fhe_N)
dbg_data = ccs0302_tasks_fast.read_serial (ccs0302_header.GPCFG_N_ADDR[0])
dbg_data = int(dbg_data, 16)
print(" << DBG: Modulus : %d" % dbg_data)
ccs0302_tasks_fast.write_serial_128 (ccs0302_header.GPCFG_NINV_ADDR[0],     ccs0302_init_val.fhe_NINV)
#ccs0302_tasks_fast.write_serial     (ccs0302_header.GPCFG_FHECTL_ADDR,      ccs0302_init_val.fhe_ctl)
#ccs0302_tasks_fast.write_serial     (ccs0302_header.GPCFG_FHECTL2,          ccs0302_init_val.fhe_ctl2)
#ccs0302_tasks_fast.write_serial     (ccs0302_header.GPCFG_FHECTL3,          ccs0302_init_val.fhe_ctl2)
#ccs0302_tasks_fast.write_serial     (ccs0201_header.GPCFG_FHECTLP_ADDR,    "00000200")
ccs0302_tasks_fast.write_serial_256    (ccs0302_header.GPCFG_NSQ_ADDR[0],   ccs0302_init_val.fhe_bmd)      #barrett
ccs0302_tasks_fast.write_serial        (ccs0302_header.GPCFG_NSQ_ADDR[5],   ccs0302_init_val.fhe_bk[2:])   #barrett
