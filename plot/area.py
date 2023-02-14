# pip install squarify
import squarify
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.font_manager import FontProperties

# Data
df_raw = pd.read_csv("https://github.com/selva86/datasets/raw/master/mpg_ggplot2.csv")

area = {
    'SRAM DP|0' : 1783551,
    'SRAM DP|1' : 1783497,
    'SRAM DP|2' : 1783581,
    'SRAM|3'    :  800904,
    'SRAM|4'    :  800896,
    'SRAM|5'    :  800924,
    'SRAM|6'    :  800900,
    'PE'        :  639384,
    'SRAM CM0'  :  406222,
    'AHB'       :   74692,
    'GPCFG'     :   53374,
    'Cortex-M0' :   35422,
    'MDMC'      :   27281,
    'SPI'       :   20024,
    'DMA'       :    7545,
    'UART'      :    6467,
    'GPIO'      :    3531
}

total = 9834470

# Prepare Data
threshold = 10**5 # um2
um2tomm2 = 10**-6
unit = '$mm^2$'

def prepare(dictionary, total, threshold, conversion, unit):
    total *= conversion
    labels = []
    data = []
    sum = 0
    for key, value in dictionary.items():
        if value >= threshold:
            data.append(value * conversion)
            labels.append(
                '{}\n{:.2f}%\n{:.2f} {}'.format(
                    key.split('|')[0],
                    (100*data[-1]/total),
                    data[-1],
                    unit
                )
            )
            sum += data[-1]
    others = total - sum
    if others:
        data.append(others)
        labels.append(
            'Others\n{:.2f}%\n{:.2f} {}'.format(
                (100*others/total),
                others,
                unit
            )
        )
    return labels, data

labels, data = prepare(area, total, threshold, um2tomm2, unit)
print(labels)
print(data)

colors = [plt.cm.Spectral(i/float(len(labels))) for i in range(len(labels))]

# Draw Plot
plt.rcParams.update({'font.size': 12})
fig, ax = plt.subplots(figsize=(12,4))

# plt.figure(figsize=(12,8), dpi= 80)
squarify.plot(sizes=data, label=labels, color=colors, alpha=.8)

# Decorate
# plt.title('Treemap of Vechile Class')
plt.axis('off')
plt.tight_layout(h_pad=0.9)
fig.savefig('cofhee_area.png')

plt.show()
