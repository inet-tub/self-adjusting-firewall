import numpy as np
import math
import random
import pandas as pd
import sys


f=str(sys.argv[1])

df=pd.read_csv(f,delimiter=' ',header=None,usecols=[0],names=["list"])

print("mean "+str(np.mean(df["list"])))
print("min "+str(np.min(df["list"])))
print("max "+str(np.max(df["list"])))