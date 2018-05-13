from Layer import  Layer
from math import  exp
class Network:
    def __init__(self,num_of_neuron,num_of_input,num_of_class,num_of_hidden_layer):
        self.layers=[]
        input_layer=Layer(num_of_neuron,num_of_input+1);

        self.layers.append(input_layer)
        for i in range(num_of_hidden_layer):
             layer=Layer(num_of_neuron,num_of_neuron)
             self.layers.append(layer)
        output_layer=Layer(num_of_class,num_of_neuron)
        self.layers.append(output_layer)

        self.num_of_neuron=num_of_neuron
        self.num_of_input=num_of_input
        self.num_of_class=num_of_class
        self.num_of_hidden_layer=num_of_hidden_layer

    def get_input_layer(self):
        return  self.layers[0]
    def get_output_layer(self):
        return self.layers(len(self.layers)-1)
    def __activate(self,weight,input):
        activation=weight[-1]
        for i in range(len(weight)-1):
            activation+=weight[i]*input[i]
        return activation

    def __transfer(self,activation):
        val = 1.0 / (1.0 + exp(-activation*1.5))
        #print(val)
        return val


    def __forward_propagate(self,dataset,row):
        input=dataset.iloc[row].values.tolist()
        for layer in self.layers:
            new_input=[]
            for neuron in layer.neuron_list:
                activation_value=self.__activate(neuron.weight,input)
                neuron.output=self.__transfer(activation_value)
                new_input.append(neuron.output)
            input=new_input.copy()
        return input
    def __transfer_derivative(self,output):
        val=output * (1.0 - output)
        #print(val)
        return val

    def get_missclassified(self,expected,outputs):
        count=0
        for i in range(len(outputs)):
            ind=outputs[i].index

    def __backward_propagate(self,expected):
         #for i
        for i in reversed((range(len(self.layers)))):
            layer=self.layers[i]
            errors=[]
            if i!=len(self.layers) -1:
                prev_layer=self.layers[i+1]
                for j  in range(len(layer.neuron_list)):
                    error=0.0
                    for prev_neuron in prev_layer.neuron_list:
                        error+=prev_neuron.weight[j]*prev_neuron.delta
                    errors.append(error)
            else:
                for j in range(len(layer.neuron_list)):

                    errors.append(expected[j]-layer.neuron_list[j].output)
            for j in range(len(layer.neuron_list)):
                    layer.neuron_list[j].delta=errors[j]*self.__transfer_derivative(layer.neuron_list[j].output)
    def __update_weights(self,row,l_rate,dataset,target):
        for i in range(len(self.layers)):
            inputs=None
            if i != 0:
                inputs = [neuron.output for neuron in self.layers[i - 1].neuron_list]
            else:
                inputs = dataset.loc[:, dataset.columns != target]
                inputs = inputs.iloc[row].values.tolist()
            for neuron in self.layers[i].neuron_list:
                for j in range(len(inputs)):
                    neuron.weight[j] += l_rate * neuron.delta * inputs[j]
                neuron.weight[-1] += l_rate * neuron.delta


    def train_network(self,dataset, l_rate, n_epoch,target):
        outputs=None
        for epoch in range(n_epoch):
            sum_error = 0
            counter=0
            for row in range(len(dataset)):
                outputs = self.__forward_propagate(dataset,row)
                expected = [0 for i in range(self.num_of_class)]
                try:
                    expected[dataset[target][row]-1]=1
                except Exception:
                    print("here")
                cl=outputs.index(max(outputs))+1
                if cl!=dataset[target][row]:
                    counter+=1
                sum_error += sum([abs(expected[i] - outputs[i])  for i in range(len(expected))])
                self.__backward_propagate(expected)
                self.__update_weights(row, l_rate,dataset,target)
                #print(cl)
            #print(self.layers[0].neuron_list[0].weight)
            #print("Epoch: "+str(epoch)+" done "+"error: "+str(counter))
    def test_network(self,dataset,target):
        counter = 0
        size=len(dataset)
        for row in range(len(dataset)):
            outputs = self.__forward_propagate(dataset, row)
            cl = outputs.index(max(outputs)) + 1
            if cl != dataset[target][row]:
                counter += 1

        val=(counter/size)*100
        print("accuracy: "+str(val))

