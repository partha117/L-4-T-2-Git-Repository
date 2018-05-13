
from random import random,seed
class Neuron:

    def __init__(self,num_of_input):


        seed(10)
        self.weight=[]
        self.delta=None
        self.output=None
        self.num_of_input=num_of_input
        for i in range(num_of_input+1):
            self.weight.append(random())

    def weight_update(self,weight):
        self.weight=weight