#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Aug  5 08:27:05 2021

@author: vamsi
"""
#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Jul 28 16:04:28 2021

@author: vamsi
"""

import os
import sys
import numpy as np
import pandas as pd
# import matplotlib
# matplotlib.use('Agg')}
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from scipy.stats import cumfreq
from matplotlib.patches import Patch
from matplotlib.lines import Line2D
from matplotlib.ticker import FormatStrFormatter
import seaborn as sns; sns.set_theme()
from matplotlib.colors import LogNorm, Normalize

#%%

plotsdir="/home/vamsi/src/phd/firewall/4-paper-sigmetrics/plots/"
# plotsdir="/home/vamsi/src/dump/"

# algs=["follow","list","efficuts","hypercuts","hicuts"]
# algs=["follow","list","efficuts"]


colors={}
colors["follow"]='blue'
colors["followfast"]='tab:blue'
colors["list"]='red'
colors["efficuts"]='green'
colors["hypercuts"]='k'
colors["hicuts"]='cyan'
colors["cutsplit"]='gray'


markers={}
markers["follow"]='x'
markers["followfast"]='P'
markers["list"]='.'
markers["efficuts"]='^'
markers["hypercuts"]='s'
markers["hicuts"]='*'
markers["cutsplit"]="d"

lines={}
algs=["follow","followfast","efficuts","hypercuts","hicuts","list","cutsplit"]
for alg in algs:
    lines[alg]='-'
lines["list"]='dotted'

# markersizes={}
# algs=["follow","followfast","efficuts","hypercuts","hicuts","list"]
# for alg in algs:
#     markersizes[alg]='-'
# lines["list"]='dotted'

#%%

# locality=[0.1,0.25,0.5,0.75,1]
# locality=[1]

locality=[1,10,100,1000,10000]
sizes=[64,128,256,512,1024,2048,4096,8192]

sizestick=[64,128,256,512,1024,2048,4096,8192]
sizesStr=["64","128","256","512","1K","2K","4K","8K"]

seeds=["acl1_seed","acl2_seed","acl3_seed","acl4_seed","acl5_seed", "fw1_seed", "fw2_seed", "fw3_seed", "fw4_seed", "fw5_seed","ipc1_seed","ipc2_seed"]

algs=["follow","followfast","efficuts","hypercuts","hicuts","list","cutsplit"]


df=pd.read_csv("./all-results-mem-4.dat",delimiter=' ')

#%%
locality=[1,10,100,1000]
sizes=[64,128,256,512,1024,2048,4096,8192]

# sizestick=[64,128,256,512,1024,2048,4096,8192]
sizesStr=["64","128","256","512","1K","2K","4K","8K"]

# seeds=["acl1_seed","acl2_seed","acl3_seed","acl4_seed","acl5_seed", "fw1_seed", "fw2_seed", "fw3_seed", "fw4_seed", "fw5_seed","ipc1_seed","ipc2_seed"]

algs=["follow"]

df=pd.read_csv("./all-results-mem-4.dat",delimiter=' ')
dfdag = pd.read_csv('./dag-stats.dat',delimiter=' ')
sizePerf=np.zeros((len(sizes),len(locality)))

for size in range(len(sizes)):
    for l in range(len(locality)):
        dfalg=df[(df["ALG"]=="follow") & (df["PB"]==locality[l]) & (df["RULESETSIZE"]==sizes[size])]
        dfalg2=df[(df["ALG"]=="cutsplit") & (df["PB"]==locality[l]) & (df["RULESETSIZE"]==sizes[size])]
        # print(dfalg["AVG"]/dfalg2["AVG"])
        sizePerf[size][l]=(float(dfalg["AVG"])/float(dfalg2["AVG"]))
        # sizePerf[size][l]=(int(dfalg["AVG"]))

sns.set(font_scale=1.2)
sizePerf=np.flip(sizePerf,axis=0)
ax = sns.heatmap(sizePerf,cmap='BrBG_r',annot=True,norm=LogNorm())
ax.set_xticks(np.arange(len(locality))+0.5)
ax.set_xticklabels(["Low (b=1)","","","High (b=1K)"])
ax.set_xlabel("Traffic Locality")
ax.set_yticks(np.arange(len(sizes))+0.5)
ax.set_yticklabels(np.flip(sizesStr),rotation=0)
ax.set_ylabel("Ruleset Size")
ax.figure.tight_layout()
ax.figure.savefig(plotsdir+'heatmap-cutsplit.pdf')

plt.clf()
sizePerf=np.zeros((len(sizes),len(locality)))

for size in range(len(sizes)):
    for l in range(len(locality)):
        dfalg=df[(df["ALG"]=="follow") & (df["PB"]==locality[l]) & (df["RULESETSIZE"]==sizes[size])]
        dfalg2=df[(df["ALG"]=="efficuts") & (df["PB"]==locality[l]) & (df["RULESETSIZE"]==sizes[size])]
        # print(dfalg["AVG"]/dfalg2["AVG"])
        sizePerf[size][l]=(float(dfalg["AVG"])/float(dfalg2["AVG"]))
        # sizePerf[size][l]=(int(dfalg["AVG"]))

sns.set(font_scale=1.2)
sizePerf=np.flip(sizePerf,axis=0)
ax1 = sns.heatmap(sizePerf,cmap='BrBG_r',annot=True,norm=LogNorm())
ax1.set_xticks(np.arange(len(locality))+0.5)
ax1.set_xticklabels(["Low (b=1)","","","High (b=1K)"])
ax1.set_xlabel("Traffic Locality")
ax1.set_yticks(np.arange(len(sizes))+0.5)
ax1.set_yticklabels(np.flip(sizesStr),rotation=0)
ax1.set_ylabel("Ruleset Size")
ax1.figure.tight_layout()
ax1.figure.savefig(plotsdir+'heatmap-efficuts.pdf')


plt.clf()
sizePerf=np.zeros((len(sizes),len(locality)))

for size in range(len(sizes)):
    for l in range(len(locality)):
        dfalg=df[(df["ALG"]=="follow") & (df["PB"]==locality[l]) & (df["RULESETSIZE"]==sizes[size])]
        dfalg2=df[(df["ALG"]=="cutsplit") & (df["PB"]==locality[l]) & (df["RULESETSIZE"]==sizes[size])]
        # print(dfalg["AVG"]/dfalg2["AVG"])
        sizePerf[size][l]=(float(dfalg["AVG"])/float(dfalg2["AVG"]))
        # sizePerf[size][l]=(int(dfalg["AVG"]))

sns.set(font_scale=1.2)
sizePerf=np.flip(sizePerf,axis=0)
ax = sns.heatmap(sizePerf,cmap='BrBG_r',annot=True,norm=LogNorm())
ax.set_xticks(np.arange(len(locality))+0.5)
ax.set_xticklabels(["Low (b=1)","","","High (b=1K)"])
ax.set_xlabel("Traffic Locality")
ax.set_yticks(np.arange(len(sizes))+0.5)
ax.set_yticklabels(np.flip([int(i) for i in dfdag["DEGREE"]]),rotation=0)
ax.set_ylabel("Average node degree")
ax.figure.tight_layout()
ax.figure.savefig(plotsdir+'heatmap-cutsplit-degree.pdf')

plt.clf()
sizePerf=np.zeros((len(sizes),len(locality)))

for size in range(len(sizes)):
    for l in range(len(locality)):
        dfalg=df[(df["ALG"]=="follow") & (df["PB"]==locality[l]) & (df["RULESETSIZE"]==sizes[size])]
        dfalg2=df[(df["ALG"]=="efficuts") & (df["PB"]==locality[l]) & (df["RULESETSIZE"]==sizes[size])]
        # print(dfalg["AVG"]/dfalg2["AVG"])
        sizePerf[size][l]=(float(dfalg["AVG"])/float(dfalg2["AVG"]))
        # sizePerf[size][l]=(int(dfalg["AVG"]))

sns.set(font_scale=1.2)
sizePerf=np.flip(sizePerf,axis=0)
ax1 = sns.heatmap(sizePerf,cmap='BrBG_r',annot=True,norm=LogNorm())
ax1.set_xticks(np.arange(len(locality))+0.5)
ax1.set_xticklabels(["Low (b=1)","","","High (b=1K)"])
ax1.set_xlabel("Traffic Locality")
ax1.set_yticks(np.arange(len(sizes))+0.5)
ax1.set_yticklabels(np.flip([int(i) for i in dfdag["DEGREE"]]),rotation=0)
ax1.set_ylabel("Average node degree")
ax1.figure.tight_layout()
ax1.figure.savefig(plotsdir+'heatmap-efficuts-degree.pdf')