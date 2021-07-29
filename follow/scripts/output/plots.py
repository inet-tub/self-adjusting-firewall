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


#%%
algs=["follow","list","efficuts","hypercuts","hicuts"]

colors={}
colors["follow"]='blue'
colors["list"]='red'
colors["efficuts"]='green'
colors["hypercuts"]='k'
colors["hicuts"]='cyan'

markers={}
markers["follow"]='x'
markers["list"]='o'
markers["efficuts"]='^'
markers["hypercuts"]='s'
markers["hicuts"]='*'


locality=[0.1,0.25,0.5,0.75,1]
sizes=[64,128,256,512,1024,2048]

df=pd.read_csv("./all-results-acl1_seed.dat",delimiter=' ')
#%%

plt.rcParams.update({'font.size': 18})
for l in locality:
    fig,ax = plt.subplots(1,1)
    ax.set_xlabel("Ruleset size")
    ax.set_ylabel("Classification time\n(Traversed nodes)")
    
    for alg in algs:
        
        dfalg = df[(df["ALG"]==alg) & (df["PB"]==l)]
        ax.plot(sizes,dfalg["AVG"],label=alg,c=colors[alg],marker=markers[alg],markersize=10)
    ax.set_yscale('log',basey=10)
    ax.set_xscale('log',basex=2)
    ax.set_xticks(sizes)
    ax.set_xticklabels(sizes)
    ax.xaxis.grid(True,ls='--')
    ax.yaxis.grid(True,ls='--')
    ax.set_title("locality="+str(l))
    ax.legend(framealpha=0)