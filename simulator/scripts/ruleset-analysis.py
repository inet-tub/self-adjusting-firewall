#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Jul 20 09:20:51 2021
@author: vamsi
"""

import numpy as np
import math
import random
import pandas as pd
import networkx as nx
import sys
#%%

# READING THE RULESET

# ruleset = "./../../classbench/db_generator/MyFilters10k"
ruleset = sys.argv[1]

srcIP=list()
dstIP=list()
srcPort=list()
dstPort=list()
protocol=list()
priority=list()

prio = 0
with open(ruleset, "r") as file:
    for line in file:
        tuples=line.split('\t')
        # SRC IP
        ipSplit = tuples[0].split('/')
        ipLen = int(ipSplit[1])
        ipstr = ipSplit[0].split('@')[1].split('.')
        a=int(ipstr[0])<< 24
        b=int(ipstr[1])<< 16
        c=int(ipstr[2]) << 8
        d=int(ipstr[3])
        ip = a + b + c + d
        ipLow = ip
        ipHigh = ipLow + (1<< (32-ipLen)) - 1
        srcIP.append((ipLow,ipHigh))

        # DST IP
        ipSplit = tuples[1].split('/')
        ipLen = int(ipSplit[1])
        ipstr = ipSplit[0].split('.')
        a=int(ipstr[0])<< 24
        b=int(ipstr[1])<< 16
        c=int(ipstr[2]) << 8
        d=int(ipstr[3])
        ip = a + b + c + d
        ipLow = ip
        ipHigh = ipLow + (1<< (32-ipLen)) - 1
        dstIP.append((ipLow,ipHigh))

        # SRC Port
        portSplit = tuples[2].split(':')
        srcPort.append((int(portSplit[0]),int(portSplit[1])))

        # DST Port
        portSplit = tuples[3].split(':')
        dstPort.append((int(portSplit[0]),int(portSplit[1])))

        # PROTOCOL
        protocolSplit = tuples[4].split('/')
        protoMask = protocolSplit[1]
        proto = int(protocolSplit[0],16)
        if protoMask=="0xFF":
            protocol.append((proto,proto))
        elif protoMask == "0x00":
            protocol.append((0,255))
        else:
            print("Invalid protocol filed encountered in classbench ruleset! Exiting")
            exit

        # PRIORITY
        priority.append(prio)
        prio+=1

rules=pd.DataFrame()
rules["srcip"]=srcIP
rules["dstip"]=dstIP
rules["srcport"]=srcPort
rules["dstport"]=dstPort
rules["protocol"]=protocol
rules["priority"]=priority

# print("NumRules %d"%prio)

# #%%
#
# This is directional. Check whether rule1 is a dependency of rule 2 or not.
def checkOverlap(rule1, rule2):
    for i in range(5):
        if (rule1[i][1] < rule2[i][0]) or rule1[i][0] > rule2[i][1]:
            return False
    if rule1[5] < rule2[5]:
        return True
    else:
        return False



# print("start building G")
G = nx.DiGraph()
for i in range(prio):
    G.add_node(i)

    for j in range(i):
        rule1 = [srcIP[j],dstIP[j],srcPort[j],dstPort[j],protocol[j],priority[j]]
        rule2 = [srcIP[i],dstIP[i],srcPort[i],dstPort[i],protocol[i],priority[i]]
        if checkOverlap(rule1,rule2):
            G.add_edge(i,j)
#%%


# print("Start computing the longest path / depth")
depth=nx.dag_longest_path(G)
# print("Depth %d"%len(depth))

degrees = [G.out_degree(n) for n in G.nodes()]
ancestors = [len(nx.ancestors(G,n)) for n in G.nodes()]

print("AverageDegree %f Depth %d NumRules %d Ancestors %f"%(np.mean(degrees),len(depth),prio,np.mean(ancestors)))
