import decimal
import sympy
import random
import math

decimal.getcontext().prec = 30

NITER   = 30
nbits   = 128
#modulus = 2**10-15
modulus = 2**nbits-1
#print ("modulus :"   , modulus)
#modulus = 9377451008442695681
#modulus = 65537
#modulus = decimal.Decimal(2**nbits-1)
#modulus = 2**128-1

#barretk   = 1 + math.floor(float('{0:.100f}'.format(math.log(modulus, 2))));

barretk     = math.ceil(math.log(modulus, 2))
nbits       = math.ceil(math.log(modulus, 2))
exp2_k      = 2**(2*barretk)
md_param_d  = math.floor((2**(2*barretk))//modulus)
md_param    = math.floor((2**(2*barretk))/modulus)
error       = '{0:.100f}'.format((2**(2*barretk)/md_param))

print ("modulus     :", modulus)
print ("nbits       :", nbits)
print ("barretk     :", barretk)
print ("barretmd_d  :", md_param_d)
print ("exp2_k      :", exp2_k)
print ("barretmd    :", md_param)
print ("Error       :", error)

nmod = 2**(barretk+1)
print ("nmod :",   nmod)

arga  = 50668
argb  = 5839
#arga  = 52
#argb  = 33

print ("-------------------------------------")
print ("Step1 : Normal Multiplication :")
y_nom = arga*argb
print ("y_nom             :", y_nom)
emod  = y_nom % modulus
print ("Expected Result   :", emod)
print ("modulus           :", modulus)
print ("-------------------------------------")


#Algorithm 1
print ("----------Algorithm 1--------------")
y_loc  = y_nom*md_param_d
print ("----------Shift", y_loc, "by--------------", 2*barretk)
#y_loc  = math.floor(y_nom >> barretk)
#y_loc2 = math.floor(y_nom//2**barretk)
print (" >> y_loc  :",   y_loc)

y_loc_shftd = math.floor(y_loc >> 2*barretk)
print ("y_loc_shftd :",   y_loc_shftd)

y_red_pre = y_nom - y_loc_shftd*modulus
print ("y_red_pre :",   y_red_pre)

y_red = y_red_pre - modulus
print ("y_red :",   y_red)

while y_red > modulus :
   y_red = y_red - modulus

print ("y_out :",   y_red)



#Algorithm 2

#//Barrett Reduction:
#// For modulus Size n bits
#// a = x*y;  //2n bits
#//High level idea is : a mod m = a - m * floor(a/m)
#//Approximate 1/m to md/2^k ; k = 2*ceil(log2 modulus) =  2*n
#// md is n+1 bits
#// a mod m = a            - m      * (a*md/2^k)
#//                 = a            - m      * (ah*2^(n+1)     + al)*md/2^2*n
#//                 = a            - m      * (ah*2^(n+1)     + al)*md/2^2*n
#//                 = a            - m      * (ah*md/2^(n-1)  + al*md/2^2*n)
#//                 = 2n bits - n bits * (n+1 bits       + 0 bit)
#//                 = a       - m*(ah*md/2^(n-1)) (edited) 

print ("----------Algorithm 2--------------")
y_nom_bin = bin(y_nom)
#Remove upper 2 char
y_nom_bin   = y_nom_bin[2:]
#y_nom_h = int(y_nom_bin_h,2)
print (" >> y_nom_bin  :",   y_nom_bin)

y_nom_h = math.floor(y_nom >> nbits-1)
print (" >> y_nom_h  :",   y_nom_h)
print (" >> y_nom_bin_h  :", bin(y_nom_h))
#Select upper n_1 bits

y_loc  = y_nom_h*md_param_d
print ("----------Shift", y_loc, "by--------------", barretk+1)
#y_loc  = math.floor(y_nom >> barretk)
#y_loc2 = math.floor(y_nom//2**barretk)
print (" >> y_loc  :",   y_loc)

y_loc_shftd = math.floor(y_loc >> (barretk+1))
print ("y_loc_shftd :",   y_loc_shftd)

print ("y_loc_shftd :",   y_loc_shftd*modulus)
print ("y_nom       :",   y_nom)
y_red_pre = y_nom - y_loc_shftd*modulus
print ("y_red_pre :",   y_red_pre)

y_red = y_red_pre
print ("y_red :",   y_red)

while y_red < 0 :
   y_red = y_red + modulus
   print ("y_red :",   y_red)

print ("y_out :",   y_red)

while y_red > modulus :
   y_red = y_red - modulus
   print ("y_red :",   y_red)

print ("y_out :",   y_red)


##Algorithm 2
#print ("----------Algorithm 2--------------")
#y_loc  = math.floor(y_nom >> barretk)*md_param
#y_loc2 = math.floor(y_nom/2**barretk)*md_param
#print ("----------Shift", y_nom, "by--------------", barretk)
##y_loc  = math.floor(y_nom >> barretk)
##y_loc2 = math.floor(y_nom//2**barretk)
#print (" >> barretk  :",   y_loc)
#print (" /2**barretk :",   y_loc2)
#print ("DIFF   :",   y_loc2 - y_loc)
#
#y_loc_shftd = math.floor(y_loc >> barretk)
#print ("y_loc_shftd :",   y_loc_shftd)
#
#y_red_pre = y_nom - y_loc_shftd*modulus
#print ("y_red_pre :",   y_red_pre)
#
#if (y_red_pre < 0) :
#  y_red = y_red_pre % nmod
#  print ("y_red :",   y_red)
#
#while y_red > modulus :
#   y_red = y_red - modulus
#
#print ("y_out :",   y_red)


###for i in range(NITER):
###   #error = float((2**barretk/md_param)) - modulus
###   error = '{0:.100f}'.format((2**barretk/md_param))
###   print ("------------------------ Iteration :", i , "------------------------")
###   print ("barretk :", barretk)
###   print ("barretmd :", md_param)
###   print ("Log2 of barretmd :", math.log(md_param, 2))
###   print ("Error :",   error)
###   barretk = barretk + 2
###   md_param = math.floor((2**barretk)/modulus)


print ("------------------------------------------------")
print ("Above Calculation done for Modulus:",   modulus)
print ("Log2 for Modulus:",   math.log(modulus, 2))
print ("------------------------------------------------")
