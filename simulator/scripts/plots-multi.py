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

#%%

df = pd.read_csv('multiThreadResults.dat',delimiter=' ')

fig, ax = plt.subplots(1,1)

dfMrf = df[df["ALG"]=="follow"]

ax.plot(dfMrf["Threads"],1/dfMrf["AvgCostPerReq"],label="MRF",lw=2, marker='s',markersize=10)
ax.plot(dfMrf["Threads"], [ 1/(list(dfMrf["AvgCostPerReq"])[0]/(i**2)) for i in dfMrf["Threads"]],label="quadratic",lw=2, marker='^',markersize=10)
ax.plot(dfMrf["Threads"], [ 1/(list(dfMrf["AvgCostPerReq"])[0]/(i)) for i in dfMrf["Threads"]],label="linear",lw=2, marker='.',markersize=10)
dfMrf = df[df["ALG"]=="efficuts"]
# ax.plot(dfMrf["Threads"],1/dfMrf["AvgCostPerReq"],label="efficuts",lw=2, marker='x',markersize=10)

ax.set_yscale('log')
ax.legend()
ax.xaxis.grid(True,ls='--')
ax.yaxis.grid(True,ls='--')
ax.set_xlabel("Threads")
ax.set_xscale('log',basex=2)
ax.set_xticks([1,2,4,8,16,32,64])
ax.set_xticklabels([1,2,4,8,16,32,64])

fig, ax = plt.subplots(1,1)

dfMrf = df[df["ALG"]=="efficuts"]

ax.plot(dfMrf["Threads"],dfMrf["AvgCostPerReq"])
ax.plot(dfMrf["Threads"], [ 1/list(dfMrf["AvgCostPerReq"])[0]/(i**2) for i in dfMrf["Threads"]],label="quadratic")
ax.plot(dfMrf["Threads"], [ 1/list(dfMrf["AvgCostPerReq"])[0]/(i) for i in dfMrf["Threads"]],label="linear")

ax.set_yscale('log')
ax.legend()
ax.xaxis.grid(True,ls='--')
ax.yaxis.grid(True,ls='--')


fig, ax = plt.subplots(1,1)

dfMrf = df[df["ALG"]=="list"]

ax.plot(dfMrf["Threads"],dfMrf["AvgCostPerReq"])
ax.plot(dfMrf["Threads"], [ 1/list(dfMrf["AvgCostPerReq"])[0]/(i**2) for i in dfMrf["Threads"]],label="quadratic")
ax.plot(dfMrf["Threads"], [ 1/list(dfMrf["AvgCostPerReq"])[0]/(i) for i in dfMrf["Threads"]],label="linear")

ax.set_yscale('log')
ax.legend()
ax.xaxis.grid(True,ls='--')
ax.yaxis.grid(True,ls='--')

fig, ax = plt.subplots(1,1)

dfMrf = df[df["ALG"]=="hypercuts"]

ax.plot(dfMrf["Threads"],dfMrf["AvgCostPerReq"])
ax.plot(dfMrf["Threads"], [ 1/list(dfMrf["AvgCostPerReq"])[0]/(i**2) for i in dfMrf["Threads"]],label="quadratic")
ax.plot(dfMrf["Threads"], [ 1/list(dfMrf["AvgCostPerReq"])[0]/(i) for i in dfMrf["Threads"]],label="linear")

ax.set_yscale('log')
ax.legend()
ax.xaxis.grid(True,ls='--')
ax.yaxis.grid(True,ls='--')