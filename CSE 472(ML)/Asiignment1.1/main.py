import pandas as pd
import matplotlib.pyplot as plt
from sklearn.model_selection import KFold
from  sklearn.model_selection import  train_test_split
import seaborn as sns
from sklearn.utils import  shuffle
from   BoostingAlgorithm.AdaBoost import  AdaBoost
df=pd.read_csv("/home/partha/MY Drives/Books/L- 4 T- 2/LAB/CSE 472(ML)/Asiignment1.1/Data/bank-additional-full.csv",sep='\s*,\s*',
                           header=0, encoding='ascii', engine='python',delimiter=';')

#print(df.describe())
#print(df.dtypes)

#print(df.dtypes)
# print(len(df.loc[df["y"]=='yes']))
# print(len(df.loc[df["y"]=='no']))
# sns.countplot("y",data=df)
# plt.show()



df_no=df.loc[df["y"]=='no']
df_yes=df.loc[df["y"]=='yes']

df_no=df_no.sample(n=(len(df_yes)))
ndf=pd.concat([df_yes,df_no])
#sns.countplot("y",data=ndf)

shuffle(ndf,random_state=20)
split_num=30
F1_sum=0
kf = KFold(n_splits=split_num,random_state=50, shuffle=True)
for train_index, test_index in kf.split(ndf):
    train_data=ndf.iloc[train_index]
    test_data=ndf.iloc[test_index]
    # train_data,test_data=train_test_split(ndf,test_size=0.2)
    a = AdaBoost(train_data, Learner="Learner.DecisionTree.DecisionLearner", limit=30)
    a.train()




    res=a.test(train_data)
    tp=fp=fn=count=0
    actual_res=train_data["y"].values.tolist()
    for j in range(0,len(actual_res)):
        if (actual_res[j] == res[j]):
            count = count + 1
        if(actual_res[j]==res[j]) and (actual_res[j]=='yes'):
            tp=tp+1
        if(actual_res[j] != res[j]) and (res[j] == 'yes'):
            fp = fp + 1
        if (actual_res[j] != res[j]) and (res[j] == 'no'):
            fn = fn + 1
    print("Train Accuracy: "+str((count/len(actual_res))*100))
    print("Train Precision: "+str(tp/(tp+fp)))
    print("Train Recall: "+str(tp/(tp+fn)))


    res=a.test(test_data)
    tp=fp=fn=count=0
    actual_res=test_data["y"].values.tolist()
    for j in range(0,len(actual_res)):
        if (actual_res[j] == res[j]):
            count = count + 1
        if (actual_res[j] == res[j]) and (actual_res[j] == 'yes'):
            tp = tp + 1
        if (actual_res[j] != res[j]) and (res[j] == 'yes'):
            fp = fp + 1
        if (actual_res[j] != res[j]) and (res[j] == 'no'):
            fn = fn + 1
    print("Test Accuracy: " + str((count / len(actual_res)) * 100))
    prec=tp / (tp + fp)
    print("Test Precision: " + str(prec))
    recall=tp / (tp + fn)
    print("Test Recall: " + str(recall))
    print("F1 Score: "+str((2/((1/prec)+(1/recall)))))
    F1_sum=F1_sum+(2/((1/prec)+(1/recall)))


    print("Learners: "+str(len(a.learners)))

    for i in range(0,len(a.learners)):
        print((a.learners[i]).attribute)

print("Average F1_score: "+str(F1_sum/split_num))


