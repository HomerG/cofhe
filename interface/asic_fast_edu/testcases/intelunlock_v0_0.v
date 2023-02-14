

//Version 0
//Foundry Unlock : Negative
uartm_write    (.addr(INTLV0_CHECK_TRIG),            .data(32'h1));

uartm_write    (.addr(INTLV0_RKEY_ADDR0),            .data(32'h0));
uartm_write    (.addr(INTLV0_RKEY_ADDR1),            .data(obf_metkey[63:32]));
uartm_write    (.addr(INTLV0_RKEY_ADDR2),            .data(obf_metkey[95:64]));
uartm_write    (.addr(INTLV0_RKEY_ADDR3),            .data(obf_metkey[127:96]));
uartm_write    (.addr(INTLV0_CHECK_TRIG),            .data(32'h2));

uartm_write    (.addr(SRAM_BASE),                    .data(32'h0123_4567));
uartm_write    (.addr(GPCFG_GPTA_CFG),               .data(32'h89AB_CDEF));
uartm_write    (.addr(GPIO_GPIO0_CTL),               .data(32'hABCD_1234));

//Version 0
//Foundry Unlock : Positive
uartm_write    (.addr(INTLV0_CHECK_TRIG),            .data(32'h1));

uartm_write    (.addr(INTLV0_RKEY_ADDR0),            .data(obf_metkey[31:0]));
uartm_write    (.addr(INTLV0_RKEY_ADDR1),            .data(obf_metkey[63:32]));
uartm_write    (.addr(INTLV0_RKEY_ADDR2),            .data(obf_metkey[95:64]));
uartm_write    (.addr(INTLV0_RKEY_ADDR3),            .data(obf_metkey[127:96]));
uartm_write    (.addr(INTLV0_CHECK_TRIG),            .data(32'h2));

uartm_write    (.addr(SRAM_BASE),                    .data(32'h0123_4567));
uartm_write    (.addr(GPCFG_GPTA_CFG),               .data(32'h89AB_CDEF));
uartm_write    (.addr(GPIO_GPIO0_CTL),               .data(32'hABCD_1234));
uartm_read     (.addr(SRAM_BASE),                    .data(rx_reg));


