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

import ccs0302_header
import ccs0302_init_val


sys.setrecursionlimit(2048)

ser = serial.Serial(
       port='/dev/tty.usbserial-210292A4C6A31',
       baudrate=4000,
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
  time.sleep(.001)
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




def mod_exp (argA, argB):
  print ("#------------------------------------------------------------------")
  print ("INFO: Performing Modular Exponentiation with following 2 arguments")
  print ("#------------------------------------------------------------------")
  print ("INFO: ARGA : %s" % argA)
  print ("INFO: ARGB : %s" % argB)
  exp_res = modexp_sw(argA,argB,ccs0302_init_val.MOD)
  argA = "{0:#0{1}x}".format(int(argA), int(ccs0302_init_val.NSQSIZE/4) + 2)
  argB = "{0:#0{1}x}".format(int(argB), int(ccs0302_init_val.NSQSIZE/4) + 2)
  write_serial_2x(ccs0302_header.GPCFG_ARGA_ADDR[0], argA)
  write_serial_2x(ccs0302_header.GPCFG_ARGB_ADDR[0], argB)
  write_serial(ccs0302_header.GPCFG_CLCTLP_ADDR,"00000002")
  time.sleep(1)
  result = read_serial_2x(ccs0302_header.GPCFG_EXP_ADDR[0])
  result = read_serial_2x(ccs0302_header.GPCFG_EXP_ADDR[0])
  result = int(result, 16)
  print ("RESULT: MOD EXP result is %d" %  result)
  if (exp_res == result):
      print ("INFO:*******************MOD_EXP PASSED************************")
  else:
    print ("ERROR:*******************MOD_EXP FAILED************************")
    print ("INFO:Expected Result %s" % exp_res)
  print ("#------------------------------------------------------------------")
  return result


#def mod_inv (argA, argB):
#  print ("#------------------------------------------------------------------")
#  print ("INFO: Performing Modular Inversion with following 2 arguments")
#  print ("#------------------------------------------------------------------")
#  print ("INFO: ARGA : %s" % argA)
#  print ("INFO: ARGB : %s" % argB)
#  exp_res = modinv_sw(argA,argB)
#  argA = "{0:#0{1}x}".format(int(argA), int(ccs0302_init_val.NSQSIZE/4) + 2)
#  argB = "{0:#0{1}x}".format(int(argB), int(ccs0302_init_val.NSQSIZE/4) + 2)
#  write_serial_2x(ccs0302_header.GPCFG_ARGA_ADDR[0], argA)
#  write_serial_2x(ccs0302_header.GPCFG_ARGB_ADDR[0], argB)
#  write_serial(ccs0302_header.GPCFG_CLCTLP_ADDR,"00000004")
#  time.sleep(1)
#  error = read_serial(ccs0302_header.GPCFG_CLSTATUS_ADDR)
#  error = bin(int(error,16))
#  error = error[-2]
#  if(error == "1"):
#    print ("WARNING: GCD is not 1")
#    result = 0
#  else:
#    result = read_serial_2x(ccs0302_header.GPCFG_INV_ADDR[0])
#    result = int(result, 16)
#    print ("RESULT: MOD INV result is %d" %  result)
#  if (exp_res == result):
#      print ("INFO:*******************MOD_INV PASSED************************")
#  else:
#    print ("ERROR:*******************MOD_INV FAILED************************")
#    print ("INFO:Expected Result %s" % exp_res)
#  print ("#------------------------------------------------------------------")
#  return error, result



def fhe_polymul ():
  print("-----Startint NTT-----------")
  lst = list(range(ccs0302_init_val.modulus-1))
  seq   = random.sample(lst, ccs0302_init_val.POLYDEG)
  seq2  = random.sample(lst, ccs0302_init_val.POLYDEG)
  fseq  = random.sample(lst, ccs0302_init_val.POLYDEG)
  fseq2 = random.sample(lst, ccs0302_init_val.POLYDEG)
  ftwdl = random.sample(lst, ccs0302_init_val.POLYDEG)
  for i in range(ccs0302_init_val.POLYDEG):
    fseq[i]  = "{0:#0{1}x}".format(int(seq[i]), int(ccs0302_init_val.NSIZE/4)+2)
    fseq2[i] = "{0:#0{1}x}".format(int(seq2[i]),int(ccs0302_init_val.NSIZE/4)+2)
    ftwdl[i] = "{0:#0{1}x}".format(int((ccs0302_init_val.nth_rou**i)%ccs0302_init_val.modulus), int(ccs0302_init_val.NSIZE/4)+2)
    #dbg_data = seq[i]
    #print(" << DBG: SEQ Value : %s" % dbg_data)
    #dbg_data = fseq[i]
    #print(" << DBG: FSEQ Value : %s" % dbg_data)
  print                (" << INFO: Loading A")
  for i in range(ccs0302_init_val.POLYDEG):
    write_serial_128 (str(hex(int(ccs0302_header.FHEMEM0_BASE,16) + 16*i)),  fseq[i])
    #dbg_data = read_serial_128  (str(hex(int(ccs0302_header.FHEMEM0_BASE,16) + 16*i)))
    #print(" << DBG: Read Data Value : %s" % dbg_data)
    #dbg_data = str(hex(int(ccs0302_header.FHEMEM0_BASE,16) + 16*i))
    #print(" << DBG: Address Value : %s" % dbg_data)
    #dbg_data = fseq[i]
    #print(" << DBG: Formatted Data Value : %s" % dbg_data)
    #dbg_data = seq[i]
    #print(" << DBG: Expected Value : %s" % dbg_data)
  #for i in range(ccs0302_init_val.POLYDEG):
  #  write_serial_128     (str(hex(int(ccs0302_header.FHEMEM1_BASE,16) + 16*i)),  fseq2[i])
  print                (" << INFO: Loading T")
  for i in range(ccs0302_init_val.POLYDEG):
    write_serial_128     (str(hex(int(ccs0302_header.FHEMEM2_BASE,16) + 16*i)),  ftwdl[i])
    #dbg_data = read_serial_128  (str(hex(int(ccs0302_header.FHEMEM2_BASE,16) + 16*i)))
    #print(" << DBG: Read Data Value TWDL : %s" % dbg_data)
    #print(" << DBG: Read Data Value TWDL int : %d" % int(dbg_data, 16))
  write_serial         (ccs0302_header.GPCFG_FHECTL2, str(ccs0302_header.FHEMEM3_BASE[:-6] +  ccs0302_header.FHEMEM2_BASE[:-6] +  ccs0302_header.FHEMEM0_BASE[:-6] + ccs0302_header.FHEMEM0_BASE[:-6]))
  write_serial         (ccs0302_header.GPCFG_FHECTL_ADDR,      (str("0040" + ccs0302_init_val.fhe_polydeg[2:])))
  dbg_data = read_serial (ccs0302_header.GPCFG_FHECTL_ADDR)
  print(" << DBG: Read Data Value POLYDEG : %s" % dbg_data)
  print(" << DBG: Read Data Value POLYDEG  : %s" % ccs0302_init_val.fhe_polydeg[2:])
  write_serial         (ccs0302_header.GPCFG_COMMNDFIFO,  str("41" + ccs0302_header.FHEMEM3_BASE[:-6] + ccs0302_header.FHEMEM0_BASE[:-6] + ccs0302_header.FHEMEM0_BASE[:-6]))
  print                (" << INFO: Triggered NTT A")
  #write_serial         (ccs0302_header.GPCFG_COMMNDFIFO,  str("41" + ccs0302_header.FHEMEM3_BASE[:-6] + ccs0302_header.FHEMEM1_BASE[:-6] + ccs0302_header.FHEMEM1_BASE[:-6]))
  #print                (" << INFO: Triggered NTT B")
  #write_serial         (ccs0302_header.GPCFG_COMMNDFIFO,  str("43" + ccs0302_header.FHEMEM3_BASE[:-6] + ccs0302_header.FHEMEM1_BASE[:-6] + ccs0302_header.FHEMEM0_BASE[:-6]))
  #print                (" << INFO: Triggered MUL A * B")
  #write_serial         (ccs0302_header.GPCFG_COMMNDFIFO,  str("46" + ccs0302_header.FHEMEM0_BASE[:-6] + ccs0302_header.FHEMEM3_BASE[:-6] + ccs0302_header.FHEMEM3_BASE[:-6]))
  #print                (" << INFO: Triggered CMUL")
  #write_serial         (ccs0302_header.GPCFG_COMMNDFIFO,  str("02" + ccs0302_header.FHEMEM1_BASE[:-6] + ccs0302_header.FHEMEM0_BASE[:-6] + ccs0302_header.FHEMEM0_BASE[:-6]))
  #print                (" << INFO: Triggered INT")
  #ntt_output    = ntt(seq,  ccs0302_init_val.modulus, ccs0302_init_val.POLYDEG, ccs0302_init_val.nth_rou)
  ntt_output = sympy.ntt(seq, ccs0302_init_val.modulus)
  #ntt_output2   = ntt(seq2, ccs0302_init_val.modulus, ccs0302_init_val.POLYDEG, ccs0302_init_val.nth_rou)
  #ntt_mul       = random.sample(lst, ccs0302_init_val.POLYDEG)  # place holder for storing the actual result
  #for i in range(ccs0302_init_val.POLYDEG):
  #  ntt_mul[i] = (ntt_output[i] * ntt_output2[i]) % ccs0302_init_val.modulus
  #intt_mul   = sympy.intt(ntt_mul, ccs0302_init_val.modulus)
  #expctd_res = random.sample(lst, ccs0302_init_val.POLYDEG)  # place holder for storing the actual result
  for i in range(ccs0302_init_val.POLYDEG):
    print(" << DBG: Expected Value : %s" % str(hex(ntt_output[i])))
  time.sleep(20)
  for i in range(ccs0302_init_val.POLYDEG):
    dbg_data = read_serial_128  (str(hex(int(ccs0302_header.FHEMEM0_BASE,16) + 16*i)))
    print(" << DBG: Read Value : %s" % dbg_data)



#def gfunc (argA, argB):
#  print ("#------------------------------------------------------------------")
#  print ("INFO: Performing GFUNCTION with following 2 arguments")
#  print ("#------------------------------------------------------------------")
#  print ("INFO: ARGA : %s" % argA)
#  print ("INFO: ARGB : %s" % argB)
#  r0n = modexp_sw(ccs0302_init_val.gfunc_r0, ccs0302_init_val.gfunc_N, ccs0302_init_val.MOD)
#  r1n = modexp_sw(ccs0302_init_val.gfunc_r1, ccs0302_init_val.gfunc_N, ccs0302_init_val.MOD)
#  x   = modexp_sw(argA, ccs0302_init_val.gfunc_fkf, ccs0302_init_val.MOD)
#  if ((x < ccs0302_init_val.gfunc_leqlo) or (x > ccs0302_init_val.gfunc_leqhi)) :
#    exp_res = r0n*r1n%ccs0302_init_val.MOD
#  else:
#    exp_res = r0n*argB%ccs0302_init_val.MOD
#  argA = "{0:#0{1}x}".format(int(argA), int(ccs0302_init_val.NSQSIZE/4) + 2)
#  argB = "{0:#0{1}x}".format(int(argB), int(ccs0302_init_val.NSQSIZE/4) + 2)
#  write_serial_2x(ccs0302_header.GPCFG_ARGA_ADDR[0], argA)
#  write_serial_2x(ccs0302_header.GPCFG_ARGB_ADDR[0], argB)
#  #write_serial(ccs0302_header.GPCFG_CLCTLP_ADDR,"00000200")
#  write_serial(ccs0302_header.GPCFG_CLCTLP_ADDR,"00000008")
#  time.sleep(2)
#  result = read_serial_2x(ccs0302_header.GPCFG_RES_ADDR[0])
#  result = read_serial_2x(ccs0302_header.GPCFG_RES_ADDR[0])
#  result = int(result, 16)
#  print ("RESULT: GFUNC result is %d" %  result)
#  if (exp_res == result):
#      print ("INFO:*******************GFUNC PASSED************************")
#  else:
#    print ("ERROR:*******************GFUNC FAILED************************")
#    print ("INFO:Expected Result %s" % exp_res)
#  print ("INFO:*******************RUNNING BBS************************")
#  ccs0302_init_val.gfunc_r0 = modexp_sw(ccs0302_init_val.gfunc_r0, 2, ccs0302_init_val.gfunc_N)
#  ccs0302_init_val.gfunc_r1 = modexp_sw(ccs0302_init_val.gfunc_r1, 2, ccs0302_init_val.gfunc_N)
#  print ("#------------------------------------------------------------------")
#  return result

#c8c1_527c_273e_6014_95f4_73bb_4c37_22a3
