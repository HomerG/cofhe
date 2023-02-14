#!/usr/bin/python

import math
import time
import random
import hashlib
import binascii

import ccs0201_init_val

#dev_ser_no            = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
#devsno_obmkey         = dev_ser_no + ccs0201_init_val.obf_mkey
#sha256_sno_obmkey     = hashlib.sha256(binascii.unhexlify(devsno_obmkey)).hexdigest()
#sha256_sno_obmkey_128 = "0x" + check[-128/4:(int(256/4))]

dev_ser_no_max        = 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
dev_ser_no            = random.randint(0, int(dev_ser_no_max))
dev_ser_no            = "{0:#0{1}x}".format(int(dev_ser_no),int(128/4)+2)
dev_ser_no_str        = str(dev_ser_no[2:])
devsno_obmkey         = dev_ser_no_str + ccs0201_init_val.obf_mkey
sha256_sno_obmkey     = hashlib.sha256(binascii.unhexlify(devsno_obmkey)).hexdigest()
sha256_sno_obmkey_128 = "0x" + sha256_sno_obmkey[-128/4:(int(256/4))]




#check = hashlib.sha256(binascii.unhexlify("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEED0BEEFEED0BEEFEED0BEEFEED0BEE")).hexdigest()
#check = hashlib.sha256("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEED0BEEFEED0BEEFEED0BEEFEED0BEE").hexdigest()
print ("INFO: Hash Calculated %s" % sha256_sno_obmkey)
print ("INFO: Hash Calculated %s" % sha256_sno_obmkey_128)
print ("Hash expected  256'h0B18_B7DC_6DC9_FEA4_619E_2656_3D47_600C_E594_E3C9_7693_6F1C_0FC4_D5E9_4FAE_FA05")
