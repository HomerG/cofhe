import math
import random
import decimal
import sympy
import math


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

print(" << DBG: Twiddle Value : %s" % int(modexp_sw(1003403946958976627,2,1152921504606584833)))
print(" << DBG: Twiddle Value : %s" % int(1003403946958976627**2%1152921504606584833))


