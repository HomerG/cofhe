#!/usr/bin/python

import math
import time
import random
import hashlib
import binascii
import datetime

import ccs0301_header
import ccs0301_tasks
import ccs0301_init_val

outFile    = "./output.log"
output_log = open(outFile, 'w')

#-------------------------------------------------------
#Connect to Serial Port
#-------------------------------------------------------
print ("INFO: Connecting to Serial Port of Cryptoleq Hardware")
print(ccs0301_tasks.ser.isOpen())

#-------------------------------------------------------
#Intel Unlock
#-------------------------------------------------------
#print ("INFO: Writing to the Intel Register")
#exec(open("intelunlock_v3_1.py").read())

#-------------------------------------------------------
#Verify Chip Signature
#-------------------------------------------------------
print ("INFO: Reading the Signature Register")
cleq_sign = ccs0301_tasks.read_serial  (ccs0301_header.GPCFG_SIGNATURE)

if (cleq_sign == "0cc50301") :
  print ("INFO: Signature Read Correctly as %s" % cleq_sign)
else :
  print ("ERROR: Signature Read Wrong as %s instead of 0CC50301" % cleq_sign)
  print ("##### Exiting the Code because of above Error")
  exit()

####--------------------------------------------------------------------
####Initialize the Cryptoleq registers
####--------------------------------------------------------------------
exec(open("fhe_init.py").read())
#print(time)
print(" << Info Time now :", datetime.datetime.now().strftime("%H:%M:%S.%f"))
ccs0301_tasks.fhe_polymul()
# ccs0301_tasks.fhe_ntt()
print(" << Info Time now :", datetime.datetime.now().strftime("%H:%M:%S.%f"))
###
####--------------------------------------------------------------------
####Run Regression
####--------------------------------------------------------------------
###exec(open("cleq_regress.py").read())
###
####--------------------------------------------------------------------
####SFLL/FLL unlock
####--------------------------------------------------------------------
###exec(open("sfll_unlock.py").read())
###
####--------------------------------------------------------------------
####Load ARM code to SRAM
####--------------------------------------------------------------------
###exec(open("arm_7seg_code.py").read())



print ("INFO: Disconnecting from Serial Port of Cryptoleq Hardware")
print(ccs0301_tasks.ser.close())
#--------------------------------------------------------------------
