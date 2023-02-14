import numpy as np
import matplotlib.pyplot as plt
from matplotlib.font_manager import FontProperties

# software power in W
software = {
    1<<12 : [ 1.32, 1.56, 1.56, 1.34, 1.34, 1.56, 1.52, 1.13, 1.22, 1.56, 1.55, 1.56, 1.56, 1.56, 1.55, 1.18, 1.55, 1.56, 1.56, 1.57 ],
    1<<13 : [ 2.51, 2.31, 2.38, 1.76, 2.44, 2.85, 2.74, 3.02, 2.10, 1.85, 1.71, 2.23, 2.08, 2.46, 2.48, 1.74, 2.04, 2.75, 2.33, 2.63 ]
}

# cofhee = software # FIXME

# prepare the data
# cf12 = np.array( cofhee[1<<12] )
sw12 = np.array( software[1<<12] )
# cf13 = np.array( cofhee[1<<13] )
sw13 = np.array( software[1<<13] )

# average
w2mw = 10**3
cf12_mean = 22.9 # np.mean(cf12)
sw12_mean = w2mw * np.mean(sw12)
cf13_mean = 21.2 # np.mean(cf13)
sw13_mean = w2mw * np.mean(sw13)

# stdev
# cf12_std = np.std(cf12)
# sw12_std = np.std(sw12)
# cf13_std = np.std(cf13)
# sw13_std = np.std(sw13)

# prepare data for plot
labels = ['$n=2^{12}$\nCoFHEE', '$n=2^{12}$\nSoftware', '$n=2^{13}$\nCoFHEE', '$n=2^{13}$\nSoftware']
x_pos = np.arange(len(labels))
means = [cf12_mean, sw12_mean, cf13_mean, sw13_mean]
print(means)
# stds  = [cf12_std, sw12_std, cf13_std, sw13_std]
width = 0.5

# build plot
plt.rcParams.update({'font.size': 12})
fig, ax = plt.subplots(figsize=(5,5))
# ax.bar(x_pos, means, width, yerr=stds, align='center', alpha=0.5, ecolor='black', capsize=10)
ax.bar(x_pos, means, width, align='center', alpha=0.5, ecolor='black', capsize=10)
ax.set_ylabel('Power (mW)')
ax.set_ylim(10**1, 10**4)
ax.set_xticks(x_pos)
ax.set_xticklabels(labels)
ax.set_title('CoFHEE vs Software (power)')
ax.yaxis.grid(True)
plt.yscale('log')

# Save figure and show
plt.tight_layout(h_pad=0.9)
plt.savefig('cofhee_power.png')
plt.show()
