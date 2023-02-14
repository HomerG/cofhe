//Version 1
//Foundry/OSAT Unlock : Negative
uartm_write    (.addr(INTLV1_RKEY_ADDR0),            .data(32'h0));
uartm_write    (.addr(INTLV1_RKEY_ADDR1),            .data(METKEY[63:32]));
uartm_write    (.addr(INTLV1_RKEY_ADDR2),            .data(METKEY[95:64]));
uartm_write    (.addr(INTLV1_RKEY_ADDR3),            .data(METKEY[127:96]));

uartm_write    (.addr(INTLV1_CHECK_TRIG),            .data(32'h2));

uartm_write    (.addr(SRAM_BASE),                    .data(32'h0123_4567));
uartm_write    (.addr(GPCFG_GPTA_CFG),               .data(32'h89AB_CDEF));
uartm_write    (.addr(GPIO_GPIO0_CTL),               .data(32'hABCD_1234));

uartm_write    (.addr(INTLV1_CHECK_TRIG),            .data(32'h2));

//Version 1
//Foundry/OSAT Unlock : Positive
uartm_write    (.addr(INTLV1_RKEY_ADDR0),            .data(METKEY[31:0]));
uartm_write    (.addr(INTLV1_RKEY_ADDR1),            .data(METKEY[63:32]));
uartm_write    (.addr(INTLV1_RKEY_ADDR2),            .data(METKEY[95:64]));
uartm_write    (.addr(INTLV1_RKEY_ADDR3),            .data(METKEY[127:96]));

uartm_write    (.addr(INTLV1_CHECK_TRIG),            .data(32'h2));

uartm_write    (.addr(SRAM_BASE),                    .data(32'h0123_4567));
uartm_write    (.addr(GPCFG_GPTA_CFG),               .data(32'h89AB_CDEF));
uartm_write    (.addr(GPIO_GPIO0_CTL),               .data(32'hABCD_1234));
uartm_read     (.addr(SRAM_BASE),                    .data(rx_reg));


