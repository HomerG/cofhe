import serial
import struct
import time
import math
import sys
import codecs
import binascii
import sympy
import random
import math
import datetime

import ccs0302_header
import ccs0302_init_val


sys.setrecursionlimit(2048)

ser = serial.Serial(
       port='/dev/tty.usbserial-FT0KRU6D',
       #port='/dev/tty.usbserial-FT0KRUOK',
       #port='/dev/tty.usbserial-FT4W405P',
       #port='/dev/ttyS4',
       #baudrate=9600,
       #baudrate=64000,
       #baudrate=256000,
       baudrate=512000,
       #baudrate=1024000,
       #baudrate=16000,
       #baudrate=1000000,
       #baudrate=500000,
       #baudrate=921600,
       #baudrate=230400,
       parity=serial.PARITY_NONE,
       stopbits=serial.STOPBITS_ONE,
       bytesize=serial.EIGHTBITS
       )


ser_uarts = serial.Serial(
       #port='/dev/tty.usbserial-FT0KRUOK',
       port='/dev/tty.usbserial-A505WVEE',
       #port='/dev/ttyS4',
       #baudrate=9600,
       #baudrate=64000,
       #baudrate=256000,
       baudrate=512000,
       #baudrate=1024000,
       #baudrate=16000,
       #baudrate=1000000,
       #baudrate=500000,
       #baudrate=921600,
       #baudrate=230400,
       parity=serial.PARITY_NONE,
       stopbits=serial.STOPBITS_ONE,
       bytesize=serial.EIGHTBITS
       )



def egcd_sw(a, b):
    if a == 0:
        return (b, 0, 1)
    else:
        g, y, x = egcd_sw(b % a, a)
        return (g, x - (b // a) * y, y)

def modinv_sw(a, m):
    g, x, y = egcd_sw(a, m)
    if g != 1:
        x = 0
    else:
        x = x % m
    return x

def bitReverse(num, len):
        """
        integer bit reverse
        input: num, bit length
        output: rev_num
        example: input 6(110) output 3(011)
        complexity: O(len)
        """
        rev_num = 0
        for i in range(0, len):
            if (num >> i) & 1:
                rev_num |= 1 << (len - 1 - i)
        return rev_num

def orderReverse(poly, N_bit):
      """docstring for order"""
      for i, coeff in enumerate(poly):
          rev_i = bitReverse(i, N_bit)
          if rev_i > i:
              coeff ^= poly[rev_i]
              poly[rev_i] ^= coeff
              coeff ^= poly[rev_i]
              poly[i] = coeff
      return poly


def ntt(poly, M, N, w):
      """number theoretic transform algorithm"""
      N_bit = N.bit_length() - 1
      #print ("poly:", poly)
      rev_poly = orderReverse(poly, N_bit)
      #print ("rev_poly:", poly)
      for i in range(0, N_bit):
          points1, points2 = [], []
          for j in range(0, int(N / 2)):
            shift_bits = N_bit - 1 - i
            P    = (j >> shift_bits) << shift_bits
            w_P  = ccs0302_init_val.modexp_sw(w,P,ccs0302_init_val.modulus) #   w ** P % M
            even = poly[2 * j]
            odd  = poly[2 * j + 1] * w_P
            #print("Stage :", i, "npoint :", j, "even :", poly[2 * j], "odd :", poly[2 * j + 1], "twidde :", w_P, "Result :", (even + odd) % M,  (even - odd) % M )
            points1.append((even + odd) % M)
            points2.append((even - odd) % M)
            # TODO: use barrett modular reduction
            points = points1 + points2
          if i != N_bit:
              poly = points
      return points



def gcd(a,b):
    while b != 0:
        a, b = b, a % b
    return a


def primRoots(modulo, cmul, nroots):
  roots = []
  hit   = 0
  for g in range(1, modulo) :
    if (hit == nroots) :
      break
    else :
      for powers in range (1, modulo) :
        pow_mod = pow(g, powers) % modulo
        if (pow_mod == 1) :
          if (powers == modulo-1) :
            if (pow(g, cmul) < modulo) :
              roots.append(pow(g, cmul))
              hit = hit + 1
          else :
            break
  return roots





def write_serial_arm(addr, data):
  wrstring  = b"\x34\x34\x34\x34"
  ser.write(wrstring)
  ser.write(addr)
  ser.write(data)

outFile    = "./output.log"
output_log = open(outFile, 'w')



def read_serial_arm(addr):
  time.sleep(.001)
  rdstring  = b"\x4D\x4D\x4D\x4D"
  ser.write(rdstring)
  ser.write(addr)
  s = ser.read(4)
  s = codecs.encode(s, "hex")
  output_log.write( str(s)  + "\n")




def read_serial(addr):
  #time.sleep(.0001)
  rdstring  = b"\x4D\x4D\x4D\x4D"
  addrstring = addr[6] + addr[7] + addr[4] + addr[5] + addr[2] + addr[3] + addr[0] + addr[1]
  addrstring    = codecs.decode(addrstring, "hex")
  ser.write(rdstring)
  ser.write(addrstring)
  s = ser.read(4)
  s = codecs.encode(s, "hex")
  s = str(s)
  s = s[8] + s[9] + s[6] + s[7] + s[4] + s[5] + s[2] + s[3]
  return s


def read_serial_uarts():
  #time.sleep(.0001)
  s = ser_uarts.read(1)
  s = codecs.encode(s, "hex")
  s = str(s)
  #s = s[8] + s[9] + s[6] + s[7] + s[4] + s[5] + s[2] + s[3]
  return s




def write_serial(addr, data):
  wrstring   = b"\x34\x34\x34\x34"
  #wrstring   = "0x34343434"
  addrstring = addr[6] + addr[7] + addr[4] + addr[5] + addr[2] + addr[3] + addr[0] + addr[1]
  addrstring    = codecs.decode(addrstring, "hex")
  #addrstring    = addrstring.decode("hex")
  datastring = data[6] + data[7] + data[4] + data[5] + data[2] + data[3] + data[0] + data[1]
  datastring = codecs.decode(datastring, "hex")
  #datastring = datastring.decode("hex")
  ser.write(wrstring)
  ser.write(addrstring)
  ser.write(datastring)

def write_serial_128(addr, data):
  data = data[2:]
  for x in range(0, int(128/32)):
    addr_loc = hex(int(addr, 16) + x*4)
    addr_loc = addr_loc[2:]
    data_loc = data[-8*x-8:(int(128/4))-8*x]
    write_serial(addr_loc, data_loc)


def read_serial_128(addr):
  s_1x = ""
  for x in range(0, int(128/32)):
    addr_loc = hex(int(addr, 16) + x*4)
    addr_loc = addr_loc[2:]
    s_1x = read_serial(addr_loc) + s_1x
  #print ("The value is %s" %  s_1x)
  return s_1x



def write_serial_256(addr, data):
  data = data[2:]
  for x in range(0, int(256/32)):
    addr_loc = hex(int(addr, 16) + x*4)
    addr_loc = addr_loc[2:]
    data_loc = data[-8*x-8:(int(256/4))-8*x]
    write_serial(addr_loc, data_loc)


def write_serial_1x(addr, data):
  data = data[2:]
  for x in range(0, int(ccs0302_init_val.NSIZE/32)):
    addr_loc = hex(int(addr, 16) + x*4)
    addr_loc = addr_loc[2:]
    data_loc = data[-8*x-8:(int(ccs0302_init_val.NSIZE/4))-8*x]
    write_serial(addr_loc, data_loc)

def write_serial_2x(addr, data):
  data = data[2:]
  for x in range(0, int(ccs0302_init_val.NSQSIZE/32)):
    addr_loc = hex(int(addr, 16) + x*4)
    addr_loc = addr_loc[2:]
    data_loc = data[-8*x-8:(int(ccs0302_init_val.NSQSIZE/4))-8*x]
    write_serial(addr_loc, data_loc)


def read_serial_1x(addr):
  s_1x = ""
  for x in range(0, int(ccs0302_init_val.NSIZE/32)):
    addr_loc = hex(int(addr, 16) + x*4)
    addr_loc = addr_loc[2:]
    s_1x = read_serial(addr_loc) + s_1x
  #print ("The value is %s" %  s_1x)
  return s_1x

def read_serial_2x(addr):
  s_2x = ""
  for x in range(0, int(ccs0302_init_val.NSQSIZE/32)):
    addr_loc = hex(int(addr, 16) + x*4)
    addr_loc = addr_loc[2:]
    s_2x = read_serial(addr_loc) + s_2x
  #print ("The value is %s" %  s_2x)
  return s_2x


def mod_mul (argA, argB):
  print ("#------------------------------------------------------------------")
  print ("INFO: Performing Modular Multiplication with following 2 arguments")
  print ("#------------------------------------------------------------------")
  print ("INFO: ARGA : %s" % argA)
  print ("INFO: ARGB : %s" % argB)
  exp_res = argA*argB%ccs0302_init_val.MOD
  argA = "{0:#0{1}x}".format(int(argA), int(ccs0302_init_val.NSQSIZE/4) + 2)
  argB = "{0:#0{1}x}".format(int(argB), int(ccs0302_init_val.NSQSIZE/4) + 2)
  write_serial_2x(ccs0302_header.GPCFG_ARGA_ADDR[0], argA)
  write_serial_2x(ccs0302_header.GPCFG_ARGB_ADDR[0], argB)
  write_serial(ccs0302_header.GPCFG_CLCTLP_ADDR,"00000001")
  result = read_serial_2x(ccs0302_header.GPCFG_MUL_ADDR[0])
  result = int(result, 16)
  print ("RESULT: MOD MUL result is %d" %  result)
  if (exp_res == result):
      print ("INFO:*******************MOD_MUL PASSED************************")
  else:
    print ("ERROR:*******************MOD_MUL FAILED************************")
    print ("INFO:Expected Result %s" % exp_res)
  print ("#------------------------------------------------------------------")
  return result




def fhe_polymul ():
  print("-----Starting POLYMUL-----------")
  print                (" << INFO: Loading Polynomials A & B")
  print(" << Info Time now :", datetime.datetime.now().strftime("%H:%M:%S.%f"))
  for i in range(ccs0302_init_val.POLYDEG):
    write_serial_128 (str(hex(int(ccs0302_header.FHEMEM0_BASE,16) + 16*i)),  ccs0302_init_val.fseq[i])
    write_serial_128 (str(hex(int(ccs0302_header.FHEMEM1_BASE,16) + 16*i)),  ccs0302_init_val.fseq2[i])
  print                (" << INFO: Done Loading Polynomials A & B")
  print(" << Info Time now :", datetime.datetime.now().strftime("%H:%M:%S.%f"))
  print                (" << INFO: Loading twiddle factor T")
  for i in range(ccs0302_init_val.POLYDEG):
    write_serial_128     (str(hex(int(ccs0302_header.FHEMEM3_BASE,16) + 16*i)),  ccs0302_init_val.ftwdl[i])
  print                (" << INFO: Done Loading twiddle factor T")
  print(" << Info Time now :", datetime.datetime.now().strftime("%H:%M:%S.%f"))
  write_serial         (ccs0302_header.GPCFG_FHECTL2,       str(ccs0302_header.FHEMEM1_BASE[:-6] +  ccs0302_header.FHEMEM3_BASE[:-6] +  ccs0302_header.FHEMEM0_BASE[:-6] + ccs0302_header.FHEMEM0_BASE[:-6]))
  write_serial         (ccs0302_header.GPCFG_FHECTL_ADDR,  (str("0040" + ccs0302_init_val.fhe_polydeg[2:])))
  write_serial         (ccs0302_header.GPCFG_COMMNDFIFO,  str("41" + ccs0302_header.FHEMEM4_BASE[:-6] + ccs0302_header.FHEMEM0_BASE[:-6] + ccs0302_header.FHEMEM0_BASE[:-6]))
  print                (" << INFO: Triggered NTT A")
  write_serial         (ccs0302_header.GPCFG_COMMNDFIFO,  str("41" + ccs0302_header.FHEMEM4_BASE[:-6] + ccs0302_header.FHEMEM1_BASE[:-6] + ccs0302_header.FHEMEM1_BASE[:-6]))
  print                (" << INFO: Triggered NTT B")
  write_serial         (ccs0302_header.GPCFG_COMMNDFIFO,  str("43" + ccs0302_header.FHEMEM4_BASE[:-6] + ccs0302_header.FHEMEM1_BASE[:-6] + ccs0302_header.FHEMEM0_BASE[:-6]))
  print                (" << INFO: Triggered MUL A * B")
  write_serial         (ccs0302_header.GPCFG_COMMNDFIFO,  str("46" + ccs0302_header.FHEMEM0_BASE[:-6] + ccs0302_header.FHEMEM4_BASE[:-6] + ccs0302_header.FHEMEM4_BASE[:-6]))
  print                (" << INFO: Triggered CMUL")
  write_serial         (ccs0302_header.GPCFG_COMMNDFIFO,  str("02" + ccs0302_header.FHEMEM1_BASE[:-6] + ccs0302_header.FHEMEM0_BASE[:-6] + ccs0302_header.FHEMEM0_BASE[:-6]))
  print                (" << INFO: Triggered INTT")
  time.sleep(2)
  print(" << Info: Reading Result : ")
  ccs0302_init_val.tempdata[0] = read_serial  (ccs0302_header.GPCFG_DBG_ADDR[1])
  ccs0302_init_val.tempdata[0] = int(("0x" + ccs0302_init_val.tempdata[0]), 0)
  print(" << Info: Total Cycles to complete Polynomial Multiplication after operands are loaded to the memory : %s" % ccs0302_init_val.tempdata[0])
  ccs0302_init_val.tempdata[0] = 25.0 * ccs0302_init_val.tempdata[0]/1000000
  print(" << Info: Total Time   to complete Polynomial Multiplication after operands are loaded to the memory : %d ms" % ccs0302_init_val.tempdata[0])
  print(" << Info Time now :", datetime.datetime.now().strftime("%H:%M:%S.%f"))
  for i in range(ccs0302_init_val.POLYDEG):
    ccs0302_init_val.tempdata[i] = read_serial_128  (str(hex(int(ccs0302_header.FHEMEM0_BASE,16) + 16*i)))
    ccs0302_init_val.tempdata[i] = int(("0x" + ccs0302_init_val.tempdata[i]), 0)
  print(" << Info: Verifying Result..............")
  print(" << Info Time now :", datetime.datetime.now().strftime("%H:%M:%S.%f"))
  ntt_output = sympy.ntt(ccs0302_init_val.seq, ccs0302_init_val.modulus)
  ntt_output2 = sympy.ntt(ccs0302_init_val.seq2, ccs0302_init_val.modulus)
  ntt_mul       = random.sample(ccs0302_init_val.lst, ccs0302_init_val.POLYDEG)  # place holder for storing the actual result
  for i in range(ccs0302_init_val.POLYDEG):
    ntt_mul[i] = (ntt_output[i] * ntt_output2[i]) % ccs0302_init_val.modulus
  intt_mul   = sympy.intt(ntt_mul, ccs0302_init_val.modulus)
  if (intt_mul  == ccs0302_init_val.tempdata) :
    print("--------------------------------------------------------------------------")
    print(" << Info: POLYNOMIAL MULTIPLICATION PASSED. ")
    print("--------------------------------------------------------------------------")
    for i in range(ccs0302_init_val.POLYDEG):
      print(" << DBG: Expected Value : %s" % str(hex(ntt_output[i])))
      print(" << DBG: Read Value : %s" % ccs0302_init_val.tempdata[i])
      print("----DBG------------------------------------------------------------------")
    print("--------------------------------------------------------------------------")
    print(" << Info: POLYNOMIAL MULTIPLICATION PASSED. ")
    print("--------------------------------------------------------------------------")
  else :
    print("--------------------------------------------------------------------------")
    print(" << ERROR: POLYNOMIAL MULTIPLICATION FAILED. ")
    print("--------------------------------------------------------------------------")
    for i in range(ccs0302_init_val.POLYDEG):
      print(" << DBG: Expected Value : %s" % str(hex(intt_mul[i])))
      print(" << DBG: Read Value : %s" % ccs0302_init_val.tempdata[i])
    print(" << ERROR: POLYNOMIAL MULTIPLICATION FAILED. ")
  print("--------------------------------------------------------------------------")


def fhe_ntt ():
  print("-----Starting NTT-----------")
  print                (" << INFO: Loading Polynomial A")
  print(" << Info Time now :", datetime.datetime.now().strftime("%H:%M:%S.%f"))
  for i in range(ccs0302_init_val.POLYDEG):
    write_serial_128 (str(hex(int(ccs0302_header.FHEMEM4_BASE,16) + 16*i)),  ccs0302_init_val.fseq[i])
    #print(" << DBG: Input Polynomial : %s" % ccs0302_init_val.fseq[i])
  print                (" << INFO: Done Loading Polynomial A")
  print(" << Info Time now :", datetime.datetime.now().strftime("%H:%M:%S.%f"))
  print                (" << INFO: Loading twiddle factor T")
  for i in range(ccs0302_init_val.POLYDEG):
    write_serial_128     (str(hex(int(ccs0302_header.FHEMEM5_BASE,16) + 16*i)),  ccs0302_init_val.ftwdl[i])
  print                (" << INFO: Done Loading twiddle factor T")
  print(" << Info Time now :", datetime.datetime.now().strftime("%H:%M:%S.%f"))
  write_serial         (ccs0302_header.GPCFG_FHECTL2,       str(ccs0302_header.FHEMEM3_BASE[:-6] +  ccs0302_header.FHEMEM5_BASE[:-6] +  ccs0302_header.FHEMEM0_BASE[:-6] + ccs0302_header.FHEMEM0_BASE[:-6]))
  write_serial         (ccs0302_header.GPCFG_FHECTL_ADDR,  (str("0040" + ccs0302_init_val.fhe_polydeg[2:])))
  write_serial         (ccs0302_header.GPCFG_COMMNDFIFO,    str("41" + ccs0302_header.FHEMEM3_BASE[:-6] + ccs0302_header.FHEMEM4_BASE[:-6] + ccs0302_header.FHEMEM4_BASE[:-6]))
  print                (" << INFO: Triggered NTT A")
  time.sleep(2)
  print(" << Reading Result : ")
  ccs0302_init_val.tempdata[0] = read_serial  (ccs0302_header.GPCFG_DBG_ADDR[1])
  ccs0302_init_val.tempdata[0] = int(("0x" + ccs0302_init_val.tempdata[0]), 0)
  print(" << Info: Total Cycles to complete NTT after operands are loaded to the memory : %s"    % ccs0302_init_val.tempdata[0])
  ccs0302_init_val.tempdata[0] = 25.0 * ccs0302_init_val.tempdata[0]/1000000
  print(" << Info: Total Time   to complete NTT after operands are loaded to the memory : %s ms" % ccs0302_init_val.tempdata[0])
  print(" << Info Time now :", datetime.datetime.now().strftime("%H:%M:%S.%f"))
  for i in range(ccs0302_init_val.POLYDEG):
    j = bitReverse(i, ccs0302_init_val.log2polydeg)
    ccs0302_init_val.tempdata[j] = read_serial_128  (str(hex(int(ccs0302_header.FHEMEM4_BASE,16) + 16*i)))
    ccs0302_init_val.tempdata[j] = int(("0x" + ccs0302_init_val.tempdata[j]), 0)
  print(" << Info: Verifying Result..............")
  print(" << Info Time now :", datetime.datetime.now().strftime("%H:%M:%S.%f"))
  ntt_output = sympy.ntt(ccs0302_init_val.seq, ccs0302_init_val.modulus)
  if (ntt_output  == ccs0302_init_val.tempdata) :
    print("--------------------------------------------------------------------------")
    print(" << Info: NTT PASSED. ")
    print("--------------------------------------------------------------------------")
    for i in range(ccs0302_init_val.POLYDEG):
      print(" << DBG: Expected Value : %s" % str(hex(ntt_output[i])))
      print(" << DBG: Read Value : %s" % ccs0302_init_val.tempdata[i])
      print("----DBG------------------------------------------------------------------")
    print("--------------------------------------------------------------------------")
    print(" << Info: NTT PASSED. ")
    print("--------------------------------------------------------------------------")
  else :
    print("--------------------------------------------------------------------------")
    print(" << ERROR : NTT FAILED. ")
    print("--------------------------------------------------------------------------")
    for i in range(ccs0302_init_val.POLYDEG):
      print(i," << DBG: Expected Value : %s" % str(hex(ntt_output[i])))
      print(" << DBG: Read Value : %s" % ccs0302_init_val.tempdata[i])
    print(" << ERROR: NTT FAILED : ")
  print(" << Info Time now :", datetime.datetime.now().strftime("%H:%M:%S.%f"))
  print("--------------------------------------------------------------------------")

def fhe_ntt_dp ():
  print("-----Starting NTT-----------")
  print                (" << INFO: Loading Polynomial A")
  print(" << Info Time now :", datetime.datetime.now().strftime("%H:%M:%S.%f"))
  for i in range(ccs0302_init_val.POLYDEG):
    write_serial_128 (str(hex(int(ccs0302_header.FHEMEM0_BASE,16) + 16*i)),  ccs0302_init_val.fseq[i])
    #print(" << DBG: Input Polynomial : %s" % ccs0302_init_val.fseq[i])
  print                (" << INFO: Done Loading Polynomial A")
  print(" << Info Time now :", datetime.datetime.now().strftime("%H:%M:%S.%f"))
  print                (" << INFO: Loading twiddle factor T")
  for i in range(ccs0302_init_val.POLYDEG):
    write_serial_128     (str(hex(int(ccs0302_header.FHEMEM5_BASE,16) + 16*i)),  ccs0302_init_val.ftwdl[i])
  print                (" << INFO: Done Loading twiddle factor T")
  print(" << Info Time now :", datetime.datetime.now().strftime("%H:%M:%S.%f"))
  write_serial         (ccs0302_header.GPCFG_FHECTL2,       str(ccs0302_header.FHEMEM3_BASE[:-6] +  ccs0302_header.FHEMEM5_BASE[:-6] +  ccs0302_header.FHEMEM0_BASE[:-6] + ccs0302_header.FHEMEM0_BASE[:-6]))
  write_serial         (ccs0302_header.GPCFG_FHECTL_ADDR,  (str("0040" + ccs0302_init_val.fhe_polydeg[2:])))
  write_serial         (ccs0302_header.GPCFG_COMMNDFIFO,    str("81"   + ccs0302_header.FHEMEM1_BASE[:-6] + ccs0302_header.FHEMEM0_DP_BASE[:-6] + ccs0302_header.FHEMEM0_BASE[:-6]))
  print                (" << INFO: Triggered NTT A")
  time.sleep(2)
  print(" << Reading Result : ")
  ccs0302_init_val.tempdata[0] = read_serial  (ccs0302_header.GPCFG_DBG_ADDR[1])
  ccs0302_init_val.tempdata[0] = int(("0x" + ccs0302_init_val.tempdata[0]), 0)
  print(" << Info: Total Cycles to complete NTT after operands are loaded to the memory : %s"    % ccs0302_init_val.tempdata[0])
  ccs0302_init_val.tempdata[0] = 25.0 * ccs0302_init_val.tempdata[0]/1000000
  print(" << Info: Total Time   to complete NTT after operands are loaded to the memory : %s ms" % ccs0302_init_val.tempdata[0])
  print(" << Info Time now :", datetime.datetime.now().strftime("%H:%M:%S.%f"))
  for i in range(ccs0302_init_val.POLYDEG):
    j = bitReverse(i, ccs0302_init_val.log2polydeg)
    ccs0302_init_val.tempdata[j] = read_serial_128  (str(hex(int(ccs0302_header.FHEMEM1_BASE,16) + 16*i)))
    ccs0302_init_val.tempdata[j] = int(("0x" + ccs0302_init_val.tempdata[j]), 0)
    time.sleep(.001)
  print(" << Info: Verifying Result..............")
  print(" << Info Time now :", datetime.datetime.now().strftime("%H:%M:%S.%f"))
  ntt_output = sympy.ntt(ccs0302_init_val.seq, ccs0302_init_val.modulus)
  #ntt_output   = ntt(ccs0302_init_val.seq, ccs0302_init_val.modulus, ccs0302_init_val.POLYDEG, ccs0302_init_val.twdl[1])
  if (ntt_output  == ccs0302_init_val.tempdata) :
    print("--------------------------------------------------------------------------")
    print(" << Info: NTT PASSED. ")
    print("--------------------------------------------------------------------------")
    #for i in range(ccs0302_init_val.POLYDEG):
    #  print(" << DBG: Expected Value : %s" % str(hex(ntt_output[i])))
    #  print(" << DBG: Read Value : %s" % ccs0302_init_val.tempdata[i])
    #  print("----DBG------------------------------------------------------------------")
    print("--------------------------------------------------------------------------")
    print(" << Info: NTT PASSED. ")
    print("--------------------------------------------------------------------------")
  else :
    print("--------------------------------------------------------------------------")
    print(" << ERROR : NTT FAILED. ")
    print("--------------------------------------------------------------------------")
    for i in range(ccs0302_init_val.POLYDEG):
      if (ntt_output[i]  != ccs0302_init_val.tempdata[i]) :
        print(i," << DBG: Expected Value : %s" % str(ntt_output[i]))
        print(i," << DBG: Expected Value : %s" % str(hex(ntt_output[i])))
        print(" << DBG: Read Value : %s" % ccs0302_init_val.tempdata[i])
        print(" << DBG: Read Value : %s" % str(hex(ccs0302_init_val.tempdata[i])))
    print(" << ERROR: NTT FAILED : ")
  print(" << Info Time now :", datetime.datetime.now().strftime("%H:%M:%S.%f"))
  print("--------------------------------------------------------------------------")



def fhe_polymul_dp ():
  print("-----Starting POLYMUL-----------")
  print                (" << INFO: Loading Polynomials A & B")
  print(" << Info Time now :", datetime.datetime.now().strftime("%H:%M:%S.%f"))
  for i in range(ccs0302_init_val.POLYDEG):
    write_serial_128 (str(hex(int(ccs0302_header.FHEMEM0_BASE,16) + 16*i)),  ccs0302_init_val.fseq[i])
    write_serial_128 (str(hex(int(ccs0302_header.FHEMEM1_BASE,16) + 16*i)),  ccs0302_init_val.fseq2[i])
  print                (" << INFO: Done Loading Polynomials A & B")
  print(" << Info Time now :", datetime.datetime.now().strftime("%H:%M:%S.%f"))
  print                (" << INFO: Loading twiddle factor T")
  for i in range(ccs0302_init_val.POLYDEG):
    write_serial_128     (str(hex(int(ccs0302_header.FHEMEM3_BASE,16) + 16*i)),  ccs0302_init_val.ftwdl[i])
  print                (" << INFO: Done Loading twiddle factor T")
  print(" << Info Time now :", datetime.datetime.now().strftime("%H:%M:%S.%f"))
  write_serial         (ccs0302_header.GPCFG_FHECTL2,       str(ccs0302_header.FHEMEM1_BASE[:-6] +  ccs0302_header.FHEMEM3_BASE[:-6] +  ccs0302_header.FHEMEM0_BASE[:-6] + ccs0302_header.FHEMEM0_BASE[:-6]))
  write_serial         (ccs0302_header.GPCFG_FHECTL_ADDR,  (str("0040" + ccs0302_init_val.fhe_polydeg[2:])))
  write_serial         (ccs0302_header.GPCFG_COMMNDFIFO,  str("81" + ccs0302_header.FHEMEM2_BASE[:-6] + ccs0302_header.FHEMEM0_DP_BASE[:-6] + ccs0302_header.FHEMEM0_BASE[:-6]))
  print                (" << INFO: Triggered NTT A")
  write_serial         (ccs0302_header.GPCFG_COMMNDFIFO,  str("81" + ccs0302_header.FHEMEM0_BASE[:-6] + ccs0302_header.FHEMEM1_DP_BASE[:-6] + ccs0302_header.FHEMEM1_BASE[:-6]))
  print                (" << INFO: Triggered NTT B")
  write_serial         (ccs0302_header.GPCFG_COMMNDFIFO,  str("43" + ccs0302_header.FHEMEM1_BASE[:-6] + ccs0302_header.FHEMEM2_BASE[:-6] + ccs0302_header.FHEMEM0_BASE[:-6]))
  print                (" << INFO: Triggered MUL A * B")
  write_serial         (ccs0302_header.GPCFG_COMMNDFIFO,  str("46" + ccs0302_header.FHEMEM0_BASE[:-6] + ccs0302_header.FHEMEM1_BASE[:-6] + ccs0302_header.FHEMEM1_BASE[:-6]))
  print                (" << INFO: Triggered CMUL")
  write_serial         (ccs0302_header.GPCFG_COMMNDFIFO,  str("82" + ccs0302_header.FHEMEM1_BASE[:-6] + ccs0302_header.FHEMEM0_DP_BASE[:-6] + ccs0302_header.FHEMEM0_BASE[:-6]))
  print                (" << INFO: Triggered INTT")
  print(" << Info Time now :", datetime.datetime.now().strftime("%H:%M:%S.%f"))
  #time.sleep(2)
  print(" << Info: Reading UARTS : ")
  ccs0302_init_val.tempdata[0]  = read_serial_uarts()
  print(" << Info: UARTS read result : %s" % ccs0302_init_val.tempdata[0])
  print(" << Info Time now :", datetime.datetime.now().strftime("%H:%M:%S.%f"))
  print(" << Info: Reading Result : ")
  ccs0302_init_val.tempdata[0] = read_serial  (ccs0302_header.GPCFG_DBG_ADDR[1])
  ccs0302_init_val.tempdata[0] = int(("0x" + ccs0302_init_val.tempdata[0]), 0)
  print(" << Info: Total Cycles to complete Polynomial Multiplication after operands are loaded to the memory : %s" % ccs0302_init_val.tempdata[0])
  ccs0302_init_val.tempdata[0] = 25.0 * ccs0302_init_val.tempdata[0]/1000000
  print(" << Info: Total Time   to complete Polynomial Multiplication after operands are loaded to the memory : %d ms" % ccs0302_init_val.tempdata[0])
  print(" << Info Time now :", datetime.datetime.now().strftime("%H:%M:%S.%f"))
  for i in range(ccs0302_init_val.POLYDEG):
    ccs0302_init_val.tempdata[i] = read_serial_128  (str(hex(int(ccs0302_header.FHEMEM1_BASE,16) + 16*i)))
    ccs0302_init_val.tempdata[i] = int(("0x" + ccs0302_init_val.tempdata[i]), 0)
  print(" << Info: Verifying Result..............")
  print(" << Info Time now :", datetime.datetime.now().strftime("%H:%M:%S.%f"))
  ntt_output = sympy.ntt(ccs0302_init_val.seq, ccs0302_init_val.modulus)
  ntt_output2 = sympy.ntt(ccs0302_init_val.seq2, ccs0302_init_val.modulus)
  ntt_mul       = random.sample(ccs0302_init_val.lst, ccs0302_init_val.POLYDEG)  # place holder for storing the actual result
  for i in range(ccs0302_init_val.POLYDEG):
    ntt_mul[i] = (ntt_output[i] * ntt_output2[i]) % ccs0302_init_val.modulus
  intt_mul   = sympy.intt(ntt_mul, ccs0302_init_val.modulus)
  if (intt_mul  == ccs0302_init_val.tempdata) :
    print("--------------------------------------------------------------------------")
    print(" << Info: POLYNOMIAL MULTIPLICATION PASSED. ")
    print("--------------------------------------------------------------------------")
    for i in range(ccs0302_init_val.POLYDEG):
      print(" << DBG: Expected Value : %s" % str(hex(intt_mul[i])))
      print(" << DBG: Read Value : %s" % ccs0302_init_val.tempdata[i])
      print("----DBG------------------------------------------------------------------")
    print("--------------------------------------------------------------------------")
    print(" << Info: POLYNOMIAL MULTIPLICATION PASSED. ")
    print("--------------------------------------------------------------------------")
  else :
    print("--------------------------------------------------------------------------")
    print(" << ERROR: POLYNOMIAL MULTIPLICATION FAILED. ")
    print("--------------------------------------------------------------------------")
    for i in range(ccs0302_init_val.POLYDEG):
      print(" << DBG: Expected Value : %s" % str(hex(intt_mul[i])))
      print(" << DBG: Read Value : %s" % ccs0302_init_val.tempdata[i])
    print(" << ERROR: POLYNOMIAL MULTIPLICATION FAILED. ")
  print("--------------------------------------------------------------------------")





