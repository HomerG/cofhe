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


#switch UART speed
print ("INFO: Switching UART speed")
#baudrate=512000,
ccs0302_tasks.write_serial (ccs0302_header.GPCFG_UARTS_BAUD_CTL,  "0000002E")
ccs0302_tasks.write_serial (ccs0302_header.GPCFG_UARTM_BAUD_CTL,  "0000002E")
#baudrate=64000,
#ccs0302_tasks.write_serial (ccs0302_header.GPCFG_UARTM_BAUD_CTL,  "0000005D")

time.sleep(2)

print ("INFO: Disconnecting from SLOWER Serial Port of FHE Hardware")
print(ccs0302_tasks.ser.close())


import ccs0302_tasks_fast


#-------------------------------------------------------
#Connect to Serial Port
#-------------------------------------------------------
print ("INFO: Connecting to Serial Port of FHE Hardware")
print(ccs0302_tasks_fast.ser.isOpen())

print ("INFO: Connecting to UARTS Serial Port of FHE Hardware")
print(ccs0302_tasks_fast.ser_uarts.isOpen())


print ("INFO: Reading the Signature Register")
cleq_sign = ccs0302_tasks_fast.read_serial  (ccs0302_header.GPCFG_SIGNATURE)

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

###print(" << Info Time now :", datetime.datetime.now().strftime("%H:%M:%S.%f"))
ccs0302_tasks_fast.fhe_polymul_dp()
#ccs0302_tasks_fast.fhe_ntt_dp()

print(" << Info Time now :", datetime.datetime.now().strftime("%H:%M:%S.%f"))


print ("INFO: Disconnecting from Serial Port of FHE Hardware")
print(ccs0302_tasks_fast.ser.close())

print ("INFO: Disconnecting from UARTS Serial Port of FHE Hardware")
print(ccs0302_tasks_fast.ser_uarts.close())
#--------------------------------------------------------------------
