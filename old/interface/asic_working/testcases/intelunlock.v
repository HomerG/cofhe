//Version 0
//Foundry Unlock : Positive
uartm_write    (.addr(INTLV0_CHECK_TRIG),            .data(32'h1));

uartm_write    (.addr(INTLV0_RKEY_ADDR0),            .data(obf_metkey[31:0]));
uartm_write    (.addr(INTLV0_RKEY_ADDR1),            .data(obf_metkey[63:32]));
uartm_write    (.addr(INTLV0_RKEY_ADDR2),            .data(obf_metkey[95:64]));
uartm_write    (.addr(INTLV0_RKEY_ADDR3),            .data(obf_metkey[127:96]));
uartm_write    (.addr(INTLV0_CHECK_TRIG),            .data(32'h2));

