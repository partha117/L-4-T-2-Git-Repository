import pandas as pd
import matplotlib.pyplot as plt
from sklearn.model_selection import KFold
from  sklearn.model_selection import  train_test_split
import seaborn as sns
from sklearn.utils import  shuffle
from   BoostingAlgorithm.AdaBoost import  AdaBoost
# df=pd.read_csv("/home/partha/MY Drives/Books/L- 4 T- 2/LAB/CSE 472(ML)/Asiignment1.1/Data/result_k.csv",sep='\s*,\s*',
#                            header=0, encoding='ascii', engine='python',delimiter=',')
#
# df.set_index('k', inplace=True)
# plt.plot(df["Stump=3"],label="Stump3")
# plt.plot(df["Stump=2"],label="Stump2")
# plt.legend(loc='upper left')
# plt.savefig("K.png")
df=pd.read_csv("/home/partha/MY Drives/Books/L- 4 T- 2/LAB/CSE 472(ML)/Asiignment1.1/Data/result_stump.csv",sep='\s*,\s*',
                           header=0, encoding='ascii', engine='python',delimiter=',')

df.set_index('stump', inplace=True)
plt.plot(df["K=10"],label="F1 Score")
plt.legend(loc='upper left')
plt.savefig("stump.png")