from Network import Network
from pandas import  read_csv
from sklearn.model_selection import train_test_split

dataset=read_csv("Iris.csv",header=0,delimiter=",")
dataset.set_index('Id', inplace=True)
input=len(dataset.iloc[0])-1
my=Network(3,input,3,1)
train,test=train_test_split(dataset,test_size=0.2)
train=train.reset_index(drop=True)
test=test.reset_index(drop=True)
my.train_network(train,0.2,350,"Species")
my.test_network(test,"Species")

