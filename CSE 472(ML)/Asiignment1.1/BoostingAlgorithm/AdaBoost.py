import pandas as pd
import matplotlib.pyplot as plt
from Learner.DecisionTree import DecisionLearner
import seaborn as sns
import  sys
import  numpy as np
import  math
import  random
class AdaBoost:
    def __get_class(self,kls):
        parts = kls.split('.')
        module = ".".join(parts[:-1])
        m = __import__(module)
        for comp in parts[1:]:
            m = getattr(m, comp)
        return m


    def __init__(self,dataFrame,Learner,limit=None):
        self.limit=limit
        self.LearnerClass=self.__get_class(Learner)
        self.actual_data=dataFrame.copy()
        self.data=dataFrame.copy()


    def __normalize(self):
        sum_val=sum(self.sample_weight)
        self.sample_weight=self.sample_weight/sum_val

    def __sample(self):
        #cum=np.zeros(len(self.actual_data))
        cum=self.sample_weight.tolist().copy()
        self.data.drop(self.data.index,inplace=True)
        for i in range(1,len(self.actual_data)):
            cum[i]=cum[i-1]+cum[i]
        for i in range(0,len(self.actual_data)):
            var=random.random()
            #self.data.iloc[i]=self.actual_data.iloc[np.where(cum>var)[0][0]-1]
            #self.data=self.data.append((self.actual_data.iloc[np.where(cum>var)[0][0]-1]).copy(),ignore_index=True)
            np_cum=np.array(cum)
            j=np.where(np_cum > var)[0][0]
            self.data = self.data.append(self.actual_data.iloc[j].copy(), ignore_index=True)




    def train(self):
            self.learners=[]
            self.learner_weight=[]
            self.sample_weight=np.ones(len(self.actual_data))/len(self.actual_data)
            #self.data = self.actual_data.sample(n=len(self.actual_data), weights=self.sample_weight)
        # if(self.limit is None):
        #     error=sys.float_info.max
        #     while(error>0.5):
        #         error=0
        #         temp_learner=self.LearnerClass()
        #         temp_learner.train(self.data)
        #         test_res=temp_learner.test(self.actual_data)
        #         self.learners.append(temp_learner)
        #         actual_res=self.data[self.data.columns[len(self.data.columns)-1]].values.tolist()
        #         for j in range(0,len(actual_res)):
        #             if(test_res[j]!=actual_res[j]):
        #                 error=error+self.sample_weight[j]
        #
        #         for j in range(0, len(actual_res)):
        #             if (test_res[j] != actual_res[j]):
        #                 self.sample_weight[j]=self.sample_weight[j]*(error/(1-error))
        #
        #
        #         self.__normalize()
        #         print("error "+str(error))
        #         self.learner_weight.append(math.log10((1-error)/error))
        #         self.__sample()
        #         #self.data = self.actual_data.sample(n=len(self.actual_data), weights=self.sample_weight)
        #         sns.countplot("y",data=self.data)
        #         plt.show()
        #
        #         print("Attribute "+temp_learner.attribute+" "+str(temp_learner))
        #
        # else:
            k=0
            while (k<self.limit):
                #self.__sample()
                #stacked=self.actual_data.stack()
                #self.data=stacked.sample(n=len(self.actual_data), weights=np.array(self.sample_weight))
                self.data = self.actual_data.sample(n=len(self.actual_data), weights=self.sample_weight)
                error = 0
                temp_learner = self.LearnerClass()
                temp_learner.train(self.data)
                test_res = temp_learner.test(self.actual_data)

                actual_res = self.data[self.data.columns[len(self.data.columns) - 1]].values.tolist()
                for j in range(0, len(actual_res)):
                    if (test_res[j] != actual_res[j]):
                        error = error + self.sample_weight[j]
                print("error:   "+str(error))
                if error<0.50:
                    self.learners.append(temp_learner)
                    for j in range(0, len(actual_res)):
                        if (test_res[j] != actual_res[j]):
                            self.sample_weight[j] = self.sample_weight[j] * (error / (1 - error))

                    self.__normalize()
                    print("error " + str(error))
                    self.learner_weight.append(math.log10((1 - error) / error))
                    #self.__sample()
                    #self.data = self.actual_data.sample(n=len(self.actual_data), weights=self.sample_weight)
                    #sns.countplot("y", data=self.data)
                    plt.show()
                    if temp_learner.numericvalue is None:
                        print("Attribute " + temp_learner.attribute + " " + str(temp_learner))
                    else:
                        print("Attribute " + temp_learner.attribute + " " + str(temp_learner)+" "+str(temp_learner.numericvalue))
                    k=k+1
                else:
                    print("error >0.5")



    def test(self,dataFrame):
        learners_result=[]
        verdicts=[]
        for i in range(0,len(self.learners)):
            learners_result.append((self.learners[i]).test(dataFrame))
        for i in range(0,len(dataFrame)):
            ultimate_verdict = 0;
            for j in range(0,len(self.learners)):
                if(learners_result[j][i]=='yes'):
                    ultimate_verdict=ultimate_verdict+self.learner_weight[j]
                else:
                    ultimate_verdict = ultimate_verdict - self.learner_weight[j]

            if(ultimate_verdict>0):
                verdicts.append("yes")
            else:
                verdicts.append("no")

        return verdicts














