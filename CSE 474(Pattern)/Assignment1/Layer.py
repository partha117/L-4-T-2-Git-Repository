
from Neuron import  Neuron
class Layer:
    def __init__(self,num_of_neuron,num_of_input):
        self.neuron_list=[]
        for i in range(num_of_neuron):
            self.neuron_list.append(Neuron(num_of_input))
        self.num_of_neuron=num_of_neuron
        self.num_of_input=num_of_input