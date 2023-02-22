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

import ccs0301_header
import ccs0301_init_val


sys.setrecursionlimit(2048)

ser = serial.Serial(
       #port='/dev/tty.usbserial-210292A4C6A31',
       port='/dev/ttyUSB1',
       #baudrate=16000,
       baudrate=1000000,
       #baudrate=1000000,
       parity=serial.PARITY_NONE,
       stopbits=serial.STOPBITS_ONE,
       bytesize=serial.EIGHTBITS
       )

def modexp_sw ( g, u, p ):
  """computes s = (g ^ u) mod p
  args are base, exponent, modulus
  (see Bruce Schneier's book, _Applied Cryptography_ p. 244)"""
  s = 1
  while u != 0:
    if u & 1:
      s = (s * g)%p
    u >>= 1
    g = (g * g)%p;
  return s

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
              P = (j >> shift_bits) << shift_bits
              w_P = w ** P % M
              even = poly[2 * j]
              odd = poly[2 * j + 1] * w_P
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
  for x in range(0, int(ccs0301_init_val.NSIZE/32)):
    addr_loc = hex(int(addr, 16) + x*4)
    addr_loc = addr_loc[2:]
    data_loc = data[-8*x-8:(int(ccs0301_init_val.NSIZE/4))-8*x]
    write_serial(addr_loc, data_loc)

def write_serial_2x(addr, data):
  data = data[2:]
  for x in range(0, int(ccs0301_init_val.NSQSIZE/32)):
    addr_loc = hex(int(addr, 16) + x*4)
    addr_loc = addr_loc[2:]
    data_loc = data[-8*x-8:(int(ccs0301_init_val.NSQSIZE/4))-8*x]
    write_serial(addr_loc, data_loc)


def read_serial_1x(addr):
  s_1x = ""
  for x in range(0, int(ccs0301_init_val.NSIZE/32)):
    addr_loc = hex(int(addr, 16) + x*4)
    addr_loc = addr_loc[2:]
    s_1x = read_serial(addr_loc) + s_1x
  #print ("The value is %s" %  s_1x)
  return s_1x

def read_serial_2x(addr):
  s_2x = ""
  for x in range(0, int(ccs0301_init_val.NSQSIZE/32)):
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
  exp_res = argA*argB%ccs0301_init_val.MOD
  argA = "{0:#0{1}x}".format(int(argA), int(ccs0301_init_val.NSQSIZE/4) + 2)
  argB = "{0:#0{1}x}".format(int(argB), int(ccs0301_init_val.NSQSIZE/4) + 2)
  write_serial_2x(ccs0301_header.GPCFG_ARGA_ADDR[0], argA)
  write_serial_2x(ccs0301_header.GPCFG_ARGB_ADDR[0], argB)
  write_serial(ccs0301_header.GPCFG_CLCTLP_ADDR,"00000001")
  result = read_serial_2x(ccs0301_header.GPCFG_MUL_ADDR[0])
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
  for i in range(ccs0301_init_val.POLYDEG):
    write_serial_128 (str(hex(int(ccs0301_header.FHEMEM0_BASE,16) + 16*i)),  ccs0301_init_val.fseq[i])
    write_serial_128 (str(hex(int(ccs0301_header.FHEMEM1_BASE,16) + 16*i)),  ccs0301_init_val.fseq2[i])
  print                (" << INFO: Done Loading Polynomials A & B")
  print(" << Info Time now :", datetime.datetime.now().strftime("%H:%M:%S.%f"))
  print                (" << INFO: Loading twiddle factor T")
  for i in range(ccs0301_init_val.POLYDEG):
    write_serial_128     (str(hex(int(ccs0301_header.FHEMEM2_BASE,16) + 16*i)),  ccs0301_init_val.ftwdl[i])
  print                (" << INFO: Done Loading twiddle factor T")
  print(" << Info Time now :", datetime.datetime.now().strftime("%H:%M:%S.%f"))
  write_serial         (ccs0301_header.GPCFG_FHECTL2,       str(ccs0301_header.FHEMEM3_BASE[:-6] +  ccs0301_header.FHEMEM2_BASE[:-6] +  ccs0301_header.FHEMEM0_BASE[:-6] + ccs0301_header.FHEMEM0_BASE[:-6]))
  # print("HERE", ccs0301_header.GPCFG_FHECTL2, str(ccs0301_header.FHEMEM3_BASE[:-6] +  ccs0301_header.FHEMEM2_BASE[:-6] +  ccs0301_header.FHEMEM0_BASE[:-6] + ccs0301_header.FHEMEM0_BASE[:-6]))
  # print(ccs0301_header.FHEMEM3_BASE[:-6],  ccs0301_header.FHEMEM2_BASE[:-6],  ccs0301_header.FHEMEM0_BASE[:-6], ccs0301_header.FHEMEM0_BASE[:-6])
  write_serial         (ccs0301_header.GPCFG_FHECTL_ADDR,  (str("0040" + ccs0301_init_val.fhe_polydeg[2:])))
  # print("GPCFG_FHECTL_ADDR", ccs0301_header.GPCFG_FHECTL_ADDR, (str("0040" + ccs0301_init_val.fhe_polydeg[2:])))
  # print("0040", ccs0301_init_val.fhe_polydeg[2:])
  write_serial         (ccs0301_header.GPCFG_COMMNDFIFO,  str("41" + ccs0301_header.FHEMEM3_BASE[:-6] + ccs0301_header.FHEMEM0_BASE[:-6] + ccs0301_header.FHEMEM0_BASE[:-6]))
  print                (" << INFO: Triggered NTT A")
  write_serial         (ccs0301_header.GPCFG_COMMNDFIFO,  str("41" + ccs0301_header.FHEMEM3_BASE[:-6] + ccs0301_header.FHEMEM1_BASE[:-6] + ccs0301_header.FHEMEM1_BASE[:-6]))
  print                (" << INFO: Triggered NTT B")
  write_serial         (ccs0301_header.GPCFG_COMMNDFIFO,  str("43" + ccs0301_header.FHEMEM3_BASE[:-6] + ccs0301_header.FHEMEM1_BASE[:-6] + ccs0301_header.FHEMEM0_BASE[:-6]))
  print                (" << INFO: Triggered MUL A * B")
  write_serial         (ccs0301_header.GPCFG_COMMNDFIFO,  str("46" + ccs0301_header.FHEMEM0_BASE[:-6] + ccs0301_header.FHEMEM3_BASE[:-6] + ccs0301_header.FHEMEM3_BASE[:-6]))
  print                (" << INFO: Triggered CMUL")
  write_serial         (ccs0301_header.GPCFG_COMMNDFIFO,  str("02" + ccs0301_header.FHEMEM1_BASE[:-6] + ccs0301_header.FHEMEM0_BASE[:-6] + ccs0301_header.FHEMEM0_BASE[:-6]))
  print                (" << INFO: Triggered INTT")
  time.sleep(2)
  print(" << Info: Reading Result : ")
  ccs0301_init_val.tempdata[0] = read_serial  (ccs0301_header.GPCFG_DBG_ADDR[1])
  ccs0301_init_val.tempdata[0] = int(("0x" + ccs0301_init_val.tempdata[0]), 0)
  print(" << Info: Total Cycles to complete Polynomial Multiplication after operands are loaded to the memory : %s" % ccs0301_init_val.tempdata[0])
  ccs0301_init_val.tempdata[0] = 25.0 * ccs0301_init_val.tempdata[0]/1000000
  print(" << Info: Total Time   to complete Polynomial Multiplication after operands are loaded to the memory : %d ms" % ccs0301_init_val.tempdata[0])
  print(" << Info Time now :", datetime.datetime.now().strftime("%H:%M:%S.%f"))
  NSIZE     = int(128)
  DWIDTH    = int(128)
  POLYDEG   = int(4096)
  with open( 'polymul.data', 'w' ) as fout:
      for i in range(ccs0301_init_val.POLYDEG):
        ccs0301_init_val.tempdata[i] = read_serial_128  (str(hex(int(ccs0301_header.FHEMEM0_BASE,16) + 16*i)))
        ccs0301_init_val.tempdata[i] = int(("0x" + ccs0301_init_val.tempdata[i]), 0)
        tmp = "{0:#0{1}x}".format(ccs0301_init_val.tempdata[i], int(NSIZE/4)+2)
        fout.write( '{}\n'.format(tmp) )
        # fout.write( '{}\n'.format(ccs0301_init_val.tempdata[i]) )
  print(" << Info: Verifying Result..............")
  print(" << Info Time now :", datetime.datetime.now().strftime("%H:%M:%S.%f"))
  ntt_output = sympy.ntt(ccs0301_init_val.seq, ccs0301_init_val.modulus)
  ntt_output2 = sympy.ntt(ccs0301_init_val.seq2, ccs0301_init_val.modulus)
  ntt_mul       = random.sample(ccs0301_init_val.lst, ccs0301_init_val.POLYDEG)  # place holder for storing the actual result
  for i in range(ccs0301_init_val.POLYDEG):
    ntt_mul[i] = (ntt_output[i] * ntt_output2[i]) % ccs0301_init_val.modulus
  intt_mul   = sympy.intt(ntt_mul, ccs0301_init_val.modulus)
  if (intt_mul  == ccs0301_init_val.tempdata) :
    print("--------------------------------------------------------------------------")
    print(" << Info: POLYNOMIAL MULTIPLICATION PASSED. ")
    print("--------------------------------------------------------------------------")
  else :
    print("--------------------------------------------------------------------------")
    print(" << ERROR: POLYNOMIAL MULTIPLICATION FAILED. ")
    print("--------------------------------------------------------------------------")
    for i in range(ccs0301_init_val.POLYDEG):
      print(" << DBG: Expected Value : %s" % str(hex(intt_mul[i])))
      print(" << DBG: Read Value : %s" % ccs0301_init_val.tempdata[i])
    print(" << ERROR: POLYNOMIAL MULTIPLICATION FAILED. ")
  print("--------------------------------------------------------------------------")


def fhe_ntt ():
  print("-----Starting NTT-----------")
  print                (" << INFO: Loading Polynomial A")
  print(" << Info Time now :", datetime.datetime.now().strftime("%H:%M:%S.%f"))
  for i in range(ccs0301_init_val.POLYDEG):
    write_serial_128 (str(hex(int(ccs0301_header.FHEMEM0_BASE,16) + 16*i)),  ccs0301_init_val.fseq[i])
  print                (" << INFO: Done Loading Polynomial A")
  print(" << Info Time now :", datetime.datetime.now().strftime("%H:%M:%S.%f"))
  print                (" << INFO: Loading twiddle factor T")
  for i in range(ccs0301_init_val.POLYDEG):
    write_serial_128     (str(hex(int(ccs0301_header.FHEMEM2_BASE,16) + 16*i)),  ccs0301_init_val.ftwdl[i])
  print                (" << INFO: Done Loading twiddle factor T")
  print(" << Info Time now :", datetime.datetime.now().strftime("%H:%M:%S.%f"))
  write_serial         (ccs0301_header.GPCFG_FHECTL2,       str(ccs0301_header.FHEMEM3_BASE[:-6] +  ccs0301_header.FHEMEM2_BASE[:-6] +  ccs0301_header.FHEMEM0_BASE[:-6] + ccs0301_header.FHEMEM0_BASE[:-6]))
  write_serial         (ccs0301_header.GPCFG_FHECTL_ADDR,  (str("0040" + ccs0301_init_val.fhe_polydeg[2:])))
  write_serial         (ccs0301_header.GPCFG_COMMNDFIFO,  str("41" + ccs0301_header.FHEMEM3_BASE[:-6] + ccs0301_header.FHEMEM0_BASE[:-6] + ccs0301_header.FHEMEM0_BASE[:-6]))
  print                (" << INFO: Triggered NTT A")
  time.sleep(2)
  print(" << Reading Result : ")
  ccs0301_init_val.tempdata[0] = read_serial  (ccs0301_header.GPCFG_DBG_ADDR[1])
  ccs0301_init_val.tempdata[0] = int(("0x" + ccs0301_init_val.tempdata[0]), 0)
  print(" << Info: Total Cycles to complete NTT after operands are loaded to the memory : %s"    %    ccs0301_init_val.tempdata[0])
  ccs0301_init_val.tempdata[0] = 25.0 * ccs0301_init_val.tempdata[0]/1000000
  print(" << Info: Total Time   to complete NTT after operands are loaded to the memory : %s ms" % ccs0301_init_val.tempdata[0])
  print(" << Info Time now :", datetime.datetime.now().strftime("%H:%M:%S.%f"))
  for i in range(ccs0301_init_val.POLYDEG):
    j = bitReverse(i, ccs0301_init_val.log2polydeg)
    ccs0301_init_val.tempdata[j] = read_serial_128  (str(hex(int(ccs0301_header.FHEMEM0_BASE,16) + 16*i)))
    ccs0301_init_val.tempdata[j] = int(("0x" + ccs0301_init_val.tempdata[j]), 0)
  print(" << Info: Verifying Result..............")
  print(" << Info Time now :", datetime.datetime.now().strftime("%H:%M:%S.%f"))
  ntt_output = sympy.ntt(ccs0301_init_val.seq, ccs0301_init_val.modulus)
  if (ntt_output  == ccs0301_init_val.tempdata) :
    print("--------------------------------------------------------------------------")
    print(" << Info: NTT PASSED. ")
    print("--------------------------------------------------------------------------")
  else :
    print("--------------------------------------------------------------------------")
    print(" << ERROR : NTT FAILED. ")
    print("--------------------------------------------------------------------------")
    for i in range(ccs0301_init_val.POLYDEG):
      print(" << DBG: Expected Value : %s" % str(hex(ntt_output[i])))
      print(" << DBG: Read Value : %s" % ccs0301_init_val.tempdata[i])
    print(" << ERROR: NTT FAILED : ")
  print(" << Info Time now :", datetime.datetime.now().strftime("%H:%M:%S.%f"))
  print("--------------------------------------------------------------------------")
