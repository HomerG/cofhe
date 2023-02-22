q = 12289
r = 41
# r = 1331
n = 4096

if r**n % q != -1:
    print("NOT")
for i in range(1, n): # [1,n)
    ri = r**i % q
    if ri == 1:
        print('{} is not a primitive root of unity because {}^{} mod {} == {}'.format(r, r, i, q, ri))
