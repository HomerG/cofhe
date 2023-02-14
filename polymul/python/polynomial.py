import numpy as np

# polynomial multiplication
def main():
    q = 77
    p1 = np.array( [ 1, 3, 1, 7, 5, 0, 9 ] )
    p2 = np.array( [ 1, 0, 1 ] )
    print( 'Polynomial: {}'.format(p1) )
    print( 'Modulus   : {}'.format(p2) )
    padd = polyadd(p1, p2, q)
    print( 'Polyadd   : {}'.format(padd) )
    pdiv = polydiv(p1, p2, q)
    print( 'Polydiv   : {}'.format(pdiv) )
    pmul = polymul(p1, p2, q)
    print( 'Polymul   : {}'.format(pmul) )
    pred = polyred(p1, p2, q)
    print( 'Polyred   : {}'.format(pred) )
    psub = polysub(p1, p2, q)
    print( 'Polysub   : {}'.format(psub) )

# element-wise modulo if q is defined
def modulo(poly, q):
    if q:
        for i in range(len(poly)):
            poly[i] = int( round( poly[i]+q if poly[i] < 0 else poly[i] ) )
            poly[i] %= q

# expects two numpy arrays and an integer
def polyadd(poly1, poly2, q):
    # conversion to poly1d
    p1 = np.poly1d(poly1)
    p2 = np.poly1d(poly2)
    # polynomial multiplication
    padd = np.polyadd(p1, p2)
    # conversion to numpy array
    aadd = np.array(padd)
    # element-wise modulo if q is defined
    modulo(aadd, q)
    return aadd

# expects two numpy arrays and an integer
def polydiv(poly1, poly2, q):
    # conversion to poly1d
    p1 = np.poly1d(poly1)
    p2 = np.poly1d(poly2)
    # polynomial multiplication
    pdiv = np.polydiv(p1, p2)[0]
    # conversion to numpy array
    adiv = np.array(pdiv)
    # element-wise modulo if q is defined
    modulo(adiv, q)
    return adiv

# expects two numpy arrays and an integer
def polymul(poly1, poly2, q=0):
    # conversion to poly1d
    p1 = np.poly1d(poly1)
    p2 = np.poly1d(poly2)
    # polynomial multiplication
    pmul = np.polymul(p1, p2)
    # conversion to numpy array
    amul = np.array(pmul)
    # element-wise modulo if q is defined
    modulo(amul, q)
    return amul

# expects two numpy arrays and an integer
def polyred(poly1, poly2, q=0):
    # conversion to poly1d
    p1 = np.poly1d(poly1)
    p2 = np.poly1d(poly2)
    # polynomial multiplication
    pmod = np.polydiv(p1, p2)[1]
    # conversion to numpy array
    amod = np.array(pmod)
    # element-wise modulo if q is defined
    modulo(amod, q)
    return amod

# expects two numpy arrays and an integer
def polysub(poly1, poly2, q=0):
    # conversion to poly1d
    p1 = np.poly1d(poly1)
    p2 = np.poly1d(poly2)
    # polynomial multiplication
    psub = np.polysub(p1, p2)
    # conversion to numpy array
    asub = np.array(psub)
    # element-wise modulo if q is defined
    modulo(asub, q)
    return asub

if __name__ == "__main__":
    main()
