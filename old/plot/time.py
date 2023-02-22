import matplotlib.pyplot as plt
from matplotlib.font_manager import FontProperties

# time in us

cycles = {     #  CF12  SW12 CF13 SW13
    'NTT'      : [ 24841, 0, 53535, 0],
    'iNTT'     : [ 29468, 0, 62770, 0],
    'PMODMUL'  : [  4627, 0,  9235, 0],
    'PMODADD'  : [  4096, 0,  8192, 0],
    # 'CMODMUL'  : [  4096, 0,  8192, 0]
}

frequency = 250 * 10**6 # Mhz
s2ms = 10**3

nOps  = {
    'NTT'     : 4,
    'iNTT'    : 3,
    'PMODMUL' : 4,
    'PMODADD' : 1,
    # 'CMODMUL' : 3
}

nTowers = [1, 3, 2, 5]

keys = list(cycles.keys())
ncols = len(cycles[keys[0]])
times = {}
for key in keys:
    times[key] = []
    for j in range(ncols):
        t = nTowers[j] * nOps[key] * cycles[key][j] * s2ms / frequency
        times[key].append(t)

# add software results
times['Software'] = [0, 2.1, 0, 8.5]
keys = list(times.keys())

print(times)

labels = ['$n=2^{12}$\nCoFHEE', '$n=2^{12}$\nSoftware', '$n=2^{13}$\nCoFHEE', '$n=2^{13}$\nSoftware']
width = 0.5       # the width of the bars: can also be len(x) sequence

plt.rcParams.update({'font.size': 12})
fig, ax = plt.subplots(figsize=(5,5))

print(ncols)
previous = ncols * [0]
print(previous)
for i, key in enumerate(keys):
    previous = [sum(x) for x in zip(previous, times[keys[i-1]] if i else ncols * [0])]
    print(i, key, previous)
    ax.bar(labels, times[key], width, bottom=previous, label=key)

ax.set_ylabel('time ($ms$)')
ax.set_title('CoFHEE vs Software (time)')
ax.legend()
plt.tight_layout(h_pad=0.9)
ax.yaxis.grid()
fig.savefig('cofhee_times.png')

plt.show()
