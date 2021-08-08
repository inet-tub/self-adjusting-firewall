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

plotsdir="/home/vamsi/src/phd/firewall/4-paper-sigmetrics/plots/"
# plotsdir="/home/vamsi/src/dump/"

# algs=["follow","list","efficuts","hypercuts","hicuts"]
# algs=["follow","list","efficuts"]


colors={}
colors["follow"]='red'
colors["followfast"]='tab:red'
colors["list"]='gray'
colors["efficuts"]='green'
colors["hypercuts"]='k'
colors["hicuts"]='cyan'
colors["cutsplit"]='y'


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
df = df[(df["PB"]==1000)]
#%%
plt.rcParams.update({'font.size': 18})
for l in locality:
    fig,ax = plt.subplots(1,1)
    ax.set_xlabel("Ruleset size")
    ax.set_ylabel("Classification time\n(Traversed nodes)")
    
    fig1,ax1 = plt.subplots(1,1)
    ax1.set_xlabel("Ruleset size")
    ax1.set_ylabel("Memory (KB)")
    
    fig2,ax2 = plt.subplots(1,1)
    ax2.set_xlabel("Memory (KB)")
    ax2.set_ylabel("Classification time\n(Traversed nodes)")
    
    for alg in algs:
        dfalg = df[(df["ALG"]==alg) & (df["PB"]==l)]
        ax.plot(sizes,dfalg["AVG"],label=alg,c=colors[alg],marker=markers[alg],markersize=10,ls=lines[alg])
        
        ax1.plot(sizes,[i/1024 for i in dfalg["MEM_AVG"]],label=alg,c=colors[alg],marker=markers[alg],markersize=10,ls=lines[alg])
        
        if alg=="list":
            continue
        ax2.plot([i/1024 for i in dfalg["MEM_AVG"]],dfalg["AVG"],label=alg,c=colors[alg],marker=markers[alg],markersize=10,ls=lines[alg])
    
    ax.set_yscale('log',basey=10)
    ax.set_xscale('log',basex=2)
    ax.set_xticks(sizestick)
    ax.set_xticklabels(sizesStr,rotation=30)
    ax.xaxis.grid(True,ls='--')
    ax.yaxis.grid(True,ls='--')
    ax.set_yticks([1,10,100,1000])
    ax.set_yticklabels([1,10,100,1000])
    # ax.set_title("locality="+str(l))
    # fig.legend(framealpha=0,ncol=2,borderpad=1)
    fig.tight_layout()
    fig.savefig(plotsdir+'time-'+str(l)+'.pdf')
    
    ax1.set_yscale('log',basey=10)
    ax1.set_xscale('log',basex=2)
    ax1.set_xticks(sizestick)
    ax1.set_xticklabels(sizesStr,rotation=30)
    ax1.xaxis.grid(True,ls='--')
    ax1.yaxis.grid(True,ls='--')
    ax1.set_yticks([1,10,100,1000])
    ax1.set_yticklabels([1,10,100,1000])
    # ax1.set_title("locality="+str(l))
    # ax1.legend(framealpha=0,ncol=3)
    fig1.tight_layout()
    fig1.savefig(plotsdir+'mem-'+str(l)+'.pdf')
    
    ax2.set_yscale('log',basey=10)
    ax2.set_xscale('log',basex=10)
    # ax2.set_xticks(sizes)
    # ax2.set_xticklabels(sizes)
    ax2.set_yticks([10,100])
    ax2.set_yticklabels([10,100])
    ax2.xaxis.grid(True,ls='--')
    ax2.yaxis.grid(True,ls='--')
    # ax2.set_title("locality="+str(l))
    ax2.legend(framealpha=0,ncol=3)
    fig2.tight_layout()
    fig2.savefig(plotsdir+'2d-'+str(l)+'.pdf')


#%%

locality=[1,10,100,1000,10000]

seeds=["acl1","acl2","acl3","acl4","acl5", "fw1", "fw2", "fw3", "fw4", "fw5","ipc1","ipc2"]

algs=["follow","efficuts","cutsplit"]

algnames={}
algnames["follow"]="Follow"
algnames["followfast"]="FollowFast"
algnames["efficuts"]="Efficuts"
algnames["cutsplit"]="CutSplit"


hatches={}
hatches["follow"]='/'
hatches["followfast"]='--'
hatches["efficuts"]='.'
hatches["cutsplit"]='--'

df=pd.read_csv("./seed-results-mem-4.dat",delimiter=' ')

plt.rcParams.update({'font.size': 24})

# plt.copper()
# plt.set_cmap('copper')
# plt.rcParams['image.cmap']='copper'

for l in [10000]:
    fig,ax = plt.subplots(1,1,figsize=(20,4))
    ax.set_ylabel("Classification time\n(Traversed nodes)")

    fig1,ax1 = plt.subplots(1,1,figsize=(20,4))
    ax1.set_ylabel("Memory (KB)")

    w=-0.2
    for alg in algs:
        dfalg = df[(df["ALG"]==alg) & (df["PB"]==l)]
    
        ax.bar(np.arange(len(seeds))+w,dfalg["AVG"],width=0.2,label=algnames[alg],hatch=hatches[alg],alpha=0.75,color=colors[alg])
        ax1.bar(np.arange(len(seeds))+w,[i/1024 for i in dfalg["MEM_AVG"]],width=0.2,label=algnames[alg],hatch=hatches[alg],alpha=0.75,color=colors[alg])
        
        w+=0.2
        
    ax.xaxis.grid(True,ls='--')
    ax.yaxis.grid(True,ls='--')
    ax.set_ylim(1,200)
    ax.set_yscale('log',basey=10)
    ax.set_yticks([5,10,100])
    ax.set_yticklabels([5,10,100])
    
    ax1.xaxis.grid(True,ls='--')
    ax1.yaxis.grid(True,ls='--')
    ax1.set_yscale('log',basey=10)
    ax1.set_yticks([1,10])
    ax1.set_yticklabels([1,10])
    ax1.set_ylim(0.4,20)
    
    ax.set_xticks(np.arange(len(seeds)))
    ax.set_xticklabels(seeds,rotation=0)
    
    ax1.set_xticks(np.arange(len(seeds)))
    ax1.set_xticklabels(seeds,rotation=0)
    
    # ax.legend(framealpha=0,ncol=3)
    # ax1.legend(framealpha=0,ncol=3)
    
    fig.tight_layout()
    fig1.tight_layout()
    
    fig.savefig(plotsdir+'seeds-time-'+str(l)+'.pdf')
    fig1.savefig(plotsdir+'seeds-mem-'+str(l)+'.pdf')
    
#%%

sizes=[64,128,256,512,1024,2048,4096,8192]

sizestick=[64,128,256,512,1024,2048,4096,8192]
plt.rcParams.update({'font.size': 14})
df = pd.read_csv('./dag-stats.dat',delimiter=' ')

fig,ax=plt.subplots(1,1)
ax.xaxis.grid(True,ls='--')
ax.yaxis.grid(True,ls='--')
ax.set_xscale('log')
ax.set_xticks(sizes)
ax.set_xticklabels(sizes,rotation=0)
ax.set_xlabel("Ruleset size")

ax.plot(df["RULESETSIZE"],df["DEPTH"],label="Depth",marker='x',c='b')
ax.plot(df["RULESETSIZE"],df["DEGREE"],label="Average node degree",marker='o',c='g')
ax.plot(df["RULESETSIZE"],df["ANCESTORS"],label="Average node ancestors",marker='^',c='r')

ax.legend(framealpha=0)

fig.savefig(plotsdir+'dag-stats.pdf')