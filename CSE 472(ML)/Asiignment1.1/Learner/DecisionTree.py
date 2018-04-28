import numpy as np
import math
import  sys
import  pandas as pd
import  random

class DecisionLearner():

    def __init__(self):
        self.Type = "Decision Tree";


    def train(self, dataFrame, weight=None,skipPoint=None):

        self.data = dataFrame
        if skipPoint is None:
            self.skip=20
        else:
            self.skip=skipPoint
        if weight is None:
            self.__weight = np.ones(len(dataFrame))
        else:
            self.__weight = np.array(weight)
        self.__startTraining()

    def test(self, dataFrame):
        test_data=dataFrame[self.attribute]
        predict=[]
        if test_data.dtype.name=='object':
            for item in test_data.values:
                try:
                    predict.append(self.criteria[item])
                except KeyError:
                    a=random.random()
                    if(a>0.5):
                        predict.append('y')
                    else:
                        predict.append('n')
        else:
            for item in test_data.values:
                if(item>=self.numericvalue):

                    predict.append(self.criteria["EorG"])
                else:
                    predict.append(self.criteria["L"])


        return predict;

    def __saveTraining(self,attribute,value):

        if (self.data[attribute].dtype.name != 'object'):
            self.numericvalue = value
        else:
            self.numericvalue =None
        self.attribute = attribute
        my=self.data[attribute]
        unique=self.data[attribute].unique()
        verdict = {}
        if (self.data[attribute].dtype.name == 'object'):
            for item in unique:
                item_frame=self.data.loc[self.data[attribute]==item]
                #res_frame=self.data[self.data.columns[len(self.data.columns)-1]].unique()

                yes=len(item_frame.loc[item_frame["y"]=='yes'])
                no = len(item_frame.loc[item_frame["y"]=='no'])
                if(yes>no):
                    verdict[item]="yes"
                else:
                    verdict[item] = "no"
        else:
            item_frame = self.data.loc[self.data[attribute] <=value]
            yes = len(item_frame.loc[item_frame["y"] == 'yes'])
            no = len(item_frame.loc[item_frame["y"] == 'no'])
            if (yes > no):
                verdict["L"] = "yes"
            else:
                verdict["L"] = "no"
            item_frame = self.data.loc[self.data[attribute] > value]
            yes = len(item_frame.loc[item_frame["y"] == 'yes'])
            no = len(item_frame.loc[item_frame["y"] == 'no'])
            if (yes > no):
                verdict["EorG"] = "yes"
            else:
                verdict["EorG"] = "no"
        self.criteria=verdict

    def __startTraining(self):
        min_entropy = 100
        min_entropy_attribute = ''
        numeric_point=0

        for i in range(0, len(self.data.columns) - 1):
            # temp_df_feature=self.data[self.data.columns[i]]
            # temp_df_result=self.data[len(self.data.columns)-1]
           # print(self.data.columns[i])
            attribute_entropy = 0
            if self.data[self.data.columns[i]].dtype.name == 'object':# Feature
                distinct_value = self.data[self.data.columns[i]].unique()
                for item in distinct_value:  #Attribute
                    current_frame = self.data.loc[self.data[self.data.columns[i]] == item]
                    distinct_value_current_last = current_frame[current_frame.columns[len(current_frame.columns) - 1]].unique()
                    entropy=0
                    for element in distinct_value_current_last:  # yes no
                        total_element = len(current_frame.index)
                        this_element = len((current_frame.loc[
                            current_frame[current_frame.columns[len(current_frame.columns) - 1]] == element]).index)
                        temp_entropy = (this_element / total_element)
                        entropy =entropy+( -temp_entropy * ((math.log(temp_entropy,2))))
                    attribute_entropy+=(total_element/(len(self.data.index)))*entropy
                #print("Attribute: "+self.data.columns[i]+" entropy:"+str(attribute_entropy))
                if attribute_entropy<min_entropy:
                        min_entropy=attribute_entropy
                        min_entropy_attribute=self.data.columns[i]


            else:

                my_frame=self.data.sort_values(self.data.columns[i])
                division_entropy=sys.float_info.max
                division_point=0
                unique_numeric_datas=my_frame[self.data.columns[i]].unique().tolist()
                for j in range(1,len(unique_numeric_datas),self.skip):
                    first_temp=my_frame.loc[my_frame[self.data.columns[i]]<unique_numeric_datas[j]]
                    last_temp = my_frame.loc[my_frame[self.data.columns[i]] >= unique_numeric_datas[j]]
                    first=(first_temp[first_temp.columns[len(first_temp.columns)-1]])
                    last=(last_temp[last_temp.columns[len(last_temp.columns)-1]])

                    unique=first.unique()
                    entropy=0
                    attribute_entropy=0
                    for item in unique:
                        temp1=first.loc[first.values == item]
                        temp_entropy=((len(temp1)/(len(first.index))))
                        entropy=entropy+(-temp_entropy*(math.log(temp_entropy,2)))
                    attribute_entropy=attribute_entropy+((len(first.index)/(len(self.data.index)))*entropy)

                    unique = last.unique()
                    entropy = 0
                    for item in unique:
                        temp1=last.loc[last.values == item]
                        temp_entropy=((len(temp1.index)/(len(last.index))))
                        entropy=entropy+(-temp_entropy*(math.log(temp_entropy,2)))
                    attribute_entropy = attribute_entropy + ((len(last.index) / (len(self.data.index))) * entropy)
                    if(attribute_entropy<division_entropy):
                        division_entropy=attribute_entropy
                        division_point=unique_numeric_datas[j]
                    #print(j)
                #print("Attribute: " + self.data.columns[i] + " entropy:" + str(division_entropy))
                if(division_entropy<min_entropy):
                    min_entropy=division_entropy
                    min_entropy_attribute=self.data.columns[i]
                    numeric_point=division_point

        self.__saveTraining(min_entropy_attribute,numeric_point)

        #print(min_entropy_attribute)
        #print(min_entropy)
        #print(numeric_point)
