import math
import sympy

def main():
    n = 4096
    bitsize = 20
    print('n: {}, bit-size: {}'.format(n, bitsize))

    # find smallest modulus
    q = find_smallest_modulus(n)
    print('q: {}'.format(q))

    # find all valid modulus within bitsize
    print('moduli: {}'.format( find_moduli(n, bitsize) ))

    # roots of unity
    roots = find_roots_of_unity(n, q)
    print('roots of unity: {}'.format(roots))

    # primitive roots of unity
    proots = find_primitive_roots_of_unity(n, q)
    print('primitive roots of unity: {}'.format(proots))

    # are all primitive roots generators?
    # primes = prime_factors(q-1)
    # for r in proots:
    #     if not is_generator_slow(r, q):
    #         print('{} is not a generator of {}'.format(r, q))

    # generators
    # generators = find_generators(q, primes)
    # print('generators: {}'.format(generators))

    # generators slow
    generators = find_generators_slow(q)
    print('generators: {}'.format(generators))

    # N-th root of unity

    # 2N-th root of unity

def find_generators(q):
    generators = []
    primes = prime_factors(q-1)
    return find_generators(q, primes)

def find_generators(q, primes):
    generators = []
    for i in range(2, q):
        if is_generator(i, q, primes):
            generators.append(i)
    return generators

def find_generators_slow(q):
    generators = []
    for i in range(2, q):
        if is_generator_slow(i, q):
            generators.append(i)
    return generators

def find_moduli(n, size):
    limit = 1 << size
    moduli = []
    k = 1
    while True:
        q = k * n + 1
        if q >= limit:
            break
        if sympy.isprime(q):
            moduli.append(q)
        k += 1
    return moduli

def find_primitive_roots_of_unity(n, q):
    proots = []
    for i in range(1, q):
        if is_primitive_root_of_unity(i, n, q):
            proots.append(i)
    return proots

def find_roots_of_unity(n, q):
    roots = []
    for i in range(1, q):
        if is_root_of_unity(i, n, q):
            roots.append(i)
    return roots

def find_smallest_modulus(n):
    k = 1
    while True:
        q = k * n + 1
        if sympy.isprime(q):
            break
        k += 1
    return q

def is_generator(g, q):
    primes = prime_factors(q-1)
    return is_generator(g, q, primes)

def is_generator(g, q, primes):
    qm1 = q-1
    primes_g = prime_factors(g)
    if g == 3:
        print(primes_g)
    for p in primes_g:
        if not qm1 % p:
            return False
    for p in primes:
        if g ** (qm1/p) % q == 1:
            return False
    return True

def is_generator_slow(g, q):
    l = []
    for i in range (1, q):
        a = g ** i % q
        if a in l:
            return False
        l.append(a)
    return True

def is_primitive_root_of_unity(z, n, q):
    if not is_root_of_unity(z, n, q):
        return False
    for m in range(1, n):
        if is_root_of_unity(z, m, q):
            return False
    return True

def is_root_of_unity(z, n, q):
    return (z ** n) % q == 1

def prime_factors(n):
    factors = []
    for i in range(2, int(math.sqrt(n))):
        isUnique = True
        while n % i == 0:
            if isUnique:
                factors.append(i)
                isUnique = False
            n /= i
    if not factors:
        factors.append(n)
    return factors

if __name__ == '__main__':
    main()
