uartm_read     (.addr(32'h400200CC),  .data(rx_reg));
//uartm_write_1k (.addr(GPCFG_N_ADDR[0][31:0]),      .data(1024'hBAD0CAD0_BAD0CAD0_BAD0CAD0_BAD0CAD0_BAD0CAD0_BAD0CAD0_BAD0CAD0_BAD0CAD0_BAD0CAD0_BAD0CAD0_BAD0CAD0_BAD0CAD0_BAD0CAD0_BAD0CAD0_BAD0CAD0_BAD0CAD0_BAD0CAD0_BAD0CAD0_BAD0CAD0_BAD0CAD0_BAD0CAD0_BAD0CAD0_BAD0CAD0_BAD0CAD0_BAD0CAD0_BAD0CAD0_BAD0CAD0_BAD0CAD0_BAD0CAD0_BAD0CAD0_BAD0CAD0_BAD0CAD0));
//uartm_read_1k  (.addr(GPCFG_N_ADDR[0][31:0]));
//uartm_write_2k (.addr(GPCFG_NSQ_ADDR[0][31:0]),    .data(2048'h0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101_0CC50101));
//uartm_read_2k  (.addr(GPCFG_NSQ_ADDR[0][31:0]));
//uartm_write_2k (.addr(GPCFG_FKF_ADDR[0][31:0]),    .data(2048'hFAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1_FAB1FAB1));
//uartm_read_2k  (.addr(GPCFG_FKF_ADDR[0][31:0]));
//uartm_write_2k (.addr(GPCFG_ARGA_ADDR[0][31:0]),   .data(2048'hAAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA_AAAAAAAA));
//uartm_read_2k  (.addr(GPCFG_ARGA_ADDR[0][31:0]));  
//uartm_write_2k (.addr(GPCFG_ARGB_ADDR[0][31:0]),   .data(2048'hBBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB_BBBBBBBB));
//uartm_read_2k  (.addr(GPCFG_ARGB_ADDR[0][31:0]));
//uartm_write_1k (.addr(GPCFG_RAND0_ADDR[0][31:0]),  .data(1024'hBAD0BAD0_BAD0BAD0_BAD0BAD0_BAD0BAD0_BAD0BAD0_BAD0BAD0_BAD0BAD0_BAD0BAD0_BAD0BAD0_BAD0BAD0_BAD0BAD0_BAD0BAD0_BAD0BAD0_BAD0BAD0_BAD0BAD0_BAD0BAD0_BAD0BAD0_BAD0BAD0_BAD0BAD0_BAD0BAD0_BAD0BAD0_BAD0BAD0_BAD0BAD0_BAD0BAD0_BAD0BAD0_BAD0BAD0_BAD0BAD0_BAD0BAD0_BAD0BAD0_BAD0BAD0_BAD0BAD0_BAD0BAD0));
//uartm_read_1k  (.addr(GPCFG_RAND0_ADDR[0][31:0]));
//uartm_write_1k (.addr(GPCFG_RAND1_ADDR[0][31:0]),  .data(1024'hBAD1BAD1_BAD1BAD1_BAD1BAD1_BAD1BAD1_BAD1BAD1_BAD1BAD1_BAD1BAD1_BAD1BAD1_BAD1BAD1_BAD1BAD1_BAD1BAD1_BAD1BAD1_BAD1BAD1_BAD1BAD1_BAD1BAD1_BAD1BAD1_BAD1BAD1_BAD1BAD1_BAD1BAD1_BAD1BAD1_BAD1BAD1_BAD1BAD1_BAD1BAD1_BAD1BAD1_BAD1BAD1_BAD1BAD1_BAD1BAD1_BAD1BAD1_BAD1BAD1_BAD1BAD1_BAD1BAD1_BAD1BAD1));
//uartm_read_1k  (.addr(GPCFG_RAND1_ADDR[0][31:0]));
//uartm_read_2k  (.addr(GPCFG_RES_ADDR[0][31:0]));

cleq_init (.n (N), .nsq (nsq), .fkf (fkf), .rand0 (rand0), .rand1 (rand1), .log2ofn(log2ofn), .maxbits (log2ofn2), .nsq_modulus (1'b1), .hw_rand (1'b1), .bypvn (1'b1));
uartm_modmul (.arga (arga),.argb (argb));
uartm_modexp (.arga (arga),.argb (argb));
uartm_modinv (.arga (arga),.argb (argb));
