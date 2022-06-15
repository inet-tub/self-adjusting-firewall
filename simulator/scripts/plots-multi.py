# -*- coding: utf-8 -*-

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

# plt.style.use('grayscale')
# plt.cm.gray
plt.rcParams.update({'font.size': 18})
#%%

df = pd.read_csv('multiThreadResults.dat',delimiter=' ')

fig, ax = plt.subplots(1,1)

dfMrf = df[df["ALG"]=="follow"]

ax.plot(dfMrf["Threads"],[list(dfMrf["AvgCostPerReq"])[0]/i for i in dfMrf["AvgCostPerReq"]],label="MRF",lw=2, marker='s',markersize=10,c="0",ls='-',)
# ax.plot(dfMrf["Threads"], [ i**2 for i in dfMrf["Threads"]],label="quadratic",lw=2, ls='--', marker='^',markersize=10,c="0.6")
ax.plot(dfMrf["Threads"], [ i for i in dfMrf["Threads"]],label="linear",lw=2, ls='--', marker='o',markersize=10,c="0.5")

# ax.set_yscale('log')
ax.legend(framealpha=0.5)
ax.xaxis.grid(True,ls='--')
ax.yaxis.grid(True,ls='--')
ax.set_xlabel("Threads")
# ax.set_xscale('log',basex=2)
# ax.set_xticks([1,2,4,8,16,32,64])
# ax.set_xticklabels([1,2,4,8,16,32,64])
# ax.set_xlim(0,32)
# ax.set_ylim(0,1000)

#%%

fig, ax = plt.subplots(1,1)

dfMrf = df[df["ALG"]=="list"]

ax.plot(dfMrf["Threads"],[list(dfMrf["AvgCostPerReq"])[0]/i for i in dfMrf["AvgCostPerReq"]],label="Static List",lw=2, marker='s',markersize=10,c="0",ls='-',)
# ax.plot(dfMrf["Threads"], [ i**2 for i in dfMrf["Threads"]],label="quadratic",lw=2, ls='--', marker='^',markersize=10,c="0.6")
ax.plot(dfMrf["Threads"], [ i for i in dfMrf["Threads"]],label="linear",lw=2, ls='--', marker='o',markersize=10,c="0.5")

# ax.set_yscale('log')
ax.legend(framealpha=0.5)
ax.xaxis.grid(True,ls='--')
ax.yaxis.grid(True,ls='--')
ax.set_xlabel("Threads")
# ax.set_xscale('log',basex=2)
# ax.set_xticks([1,2,4,8,16,32,64])
# ax.set_xticklabels([1,2,4,8,16,32,64])
# ax.set_xlim(0,32)
# ax.set_ylim(0,1000)

#%%

fig, ax = plt.subplots(1,1)

dfMrf = df[df["ALG"]=="efficuts"]

ax.plot(dfMrf["Threads"],[list(dfMrf["AvgCostPerReq"])[0]/i for i in dfMrf["AvgCostPerReq"]],label="Efficuts (Static tree-based)",lw=2, marker='s',markersize=10,c="0",ls='-',)
# ax.plot(dfMrf["Threads"], [ i**2 for i in dfMrf["Threads"]],label="quadratic",lw=2, ls='--', marker='^',markersize=10,c="0.6")
ax.plot(dfMrf["Threads"], [ i for i in dfMrf["Threads"]],label="linear",lw=2, ls='--', marker='o',markersize=10,c="0.5")

# ax.set_yscale('log')
ax.legend(framealpha=0.5)
ax.xaxis.grid(True,ls='--')
ax.yaxis.grid(True,ls='--')
ax.set_xlabel("Threads")
# ax.set_xscale('log',basex=2)
# ax.set_xticks([1,2,4,8,16,32,64])
# ax.set_xticklabels([1,2,4,8,16,32,64])
# ax.set_xlim(0,32)
# ax.set_ylim(0,1000)

#%%


fig, ax = plt.subplots(1,1)

dfMrf = df[df["ALG"]=="hypercuts"]

ax.plot(dfMrf["Threads"],[list(dfMrf["AvgCostPerReq"])[0]/i for i in dfMrf["AvgCostPerReq"]],label="Hypercuts (Static tree-based)",lw=2, marker='s',markersize=10,c="0",ls='-',)
# ax.plot(dfMrf["Threads"], [ i**2 for i in dfMrf["Threads"]],label="quadratic",lw=2, ls='--', marker='^',markersize=10,c="0.6")
ax.plot(dfMrf["Threads"], [ i for i in dfMrf["Threads"]],label="linear",lw=2, ls='--', marker='o',markersize=10,c="0.5")

# ax.set_yscale('log')
ax.legend(framealpha=0.5)
ax.xaxis.grid(True,ls='--')
ax.yaxis.grid(True,ls='--')
ax.set_xlabel("Threads")
# ax.set_xscale('log',basex=2)
# ax.set_xticks([1,2,4,8,16,32,64])
# ax.set_xticklabels([1,2,4,8,16,32,64])
# ax.set_xlim(0,32)
# ax.set_ylim(0,1000)