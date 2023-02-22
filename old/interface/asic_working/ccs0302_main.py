#!/usr/bin/python

print ("INFO: Initializing prameters for FHE Hardware")
import math
import time
import random
import hashlib
import binascii
import datetime

import ccs0302_header
import ccs0302_tasks
import ccs0302_init_val

outFile    = "./output.log"
output_log = open(outFile, 'w')

#-------------------------------------------------------
#Connect to Serial Port
#-------------------------------------------------------
print ("INFO: Connecting to Serial Port of FHE Hardware")
print(ccs0302_tasks.ser.isOpen())

#-------------------------------------------------------
#Intel Unlock
#-------------------------------------------------------
#print ("INFO: Writing to the Intel Register")
#exec(open("intelunlock_v3_1.py").read())

#-------------------------------------------------------
#Verify Chip Signature
#-------------------------------------------------------
print ("INFO: Reading the Signature Register")
cleq_sign = ccs0302_tasks.read_serial  (ccs0302_header.GPCFG_SIGNATURE)

if (cleq_sign == "0cc50302") :
  print ("INFO: Signature Read Correctly as %s" % cleq_sign)
else :
  print ("ERROR: Signature Read Wrong as %s instead of 0CC50302" % cleq_sign)
  print ("##### Exiting the Code because of above Error")
  exit()

####--------------------------------------------------------------------
####Initialize the FHE registers
####--------------------------------------------------------------------
exec(open("fhe_init.py").read())   #TODO

###ccs0302_tasks.write_serial_128 (str(hex(int(ccs0302_header.FHEMEM1_BASE,16) + 16*1)),  ccs0302_init_val.fseq[0])
#dbg_data0 = ccs0302_tasks.read_serial_128  (str(hex(int(ccs0302_header.FHEMEM0_BASE,16) + 16*0)))
#print(" << DBG: Read    Data : %s" % dbg_data0)

#dbg_data1 = ccs0302_tasks.read_serial_128  (str(hex(int(ccs0302_header.FHEMEM1_BASE,16) + 16*0)))
#print(" << DBG: Read    Data : %s" % dbg_data1)

#dbg_data2 = ccs0302_tasks.read_serial_128  (str(hex(int(ccs0302_header.FHEMEM2_BASE,16) + 16*0)))
#print(" << DBG: Read    Data : %s" % dbg_data2)

###ccs0302_tasks.write_serial ("22000000",  "DEADBEEF")
###dbg_data0 = ccs0302_tasks.read_serial("22000000")
##dbg_data1 = ccs0302_tasks.read_serial(ccs0302_header.GPCFG_NSQ_ADDR[5])   #barrett
##print(" << DBG: Read    Data : %s" % dbg_data1)

#print(time)

print(" << Info Time now :", datetime.datetime.now().strftime("%H:%M:%S.%f"))
ccs0302_tasks.fhe_polymul_dp()
#ccs0302_tasks.fhe_ntt_dp()

###i = ccs0302_tasks.bitReverse(11, ccs0302_init_val.log2polydeg)
###ccs0302_tasks.write_serial_128 (str(hex(int(ccs0302_header.FHEMEM0_BASE,16) + 16*i)),  ccs0302_init_val.fseq[0])
###dbg_data0 = ccs0302_tasks.read_serial_128  (str(hex(int(ccs0302_header.FHEMEM0_BASE,16) + 16*i)))
###print(" << DBG: Read    Data : %s" % dbg_data0)
###
###i = ccs0302_tasks.bitReverse(15, ccs0302_init_val.log2polydeg)
###dbg_data1 = ccs0302_tasks.read_serial_128  (str(hex(int(ccs0302_header.FHEMEM0_BASE,16) + 16*i)))
###print(" << DBG: Read    Data : %s" % dbg_data1)
###
###i = ccs0302_tasks.bitReverse(43, ccs0302_init_val.log2polydeg)
###dbg_data2 = ccs0302_tasks.read_serial_128  (str(hex(int(ccs0302_header.FHEMEM0_BASE,16) + 16*i)))
###print(" << DBG: Read    Data : %s" % dbg_data2)



print(" << Info Time now :", datetime.datetime.now().strftime("%H:%M:%S.%f"))


print ("INFO: Disconnecting from Serial Port of FHE Hardware")
print(ccs0302_tasks.ser.close())
#--------------------------------------------------------------------
