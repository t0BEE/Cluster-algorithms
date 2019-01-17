#import numpy as numpy
import pandas as pd
import matplotlib.pyplot as plt

df  = pd.read_csv("Sample.csv", sep=';')
beff = pd.read_csv("K-Means_Algorithm/k-Means/output0.csv", sep=';')
einsf = pd.read_csv("K-Means_Algorithm/k-Means/output0.csv", sep=';')
dreief = pd.read_csv("K-Means_Algorithm/k-Means/output2.csv", sep=';')
sechsf = pd.read_csv("K-Means_Algorithm/k-Means/output5.csv", sep=';')
einsf

beff.plot(kind='scatter', x='a', y='b', colormap=plt.get_cmap('rainbow'), c='c')
einsf.plot(kind='scatter', x='a', y='b', colormap=plt.get_cmap('rainbow'), c='c')
dreief.plot(kind='scatter', x='a', y='b', colormap=plt.get_cmap('rainbow'), c='c')
sechsf.plot(kind='scatter', x='a', y='b', colormap=plt.get_cmap('rainbow'), c='c')
plt.show()