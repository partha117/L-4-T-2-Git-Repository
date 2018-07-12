import  numpy as np
import  math
import scipy.stats

class state:
    def __init__(self,probability=0,Parent=0,maxprobability=0):
        self.probability=probability
        self.maxParent=Parent
        self.maxProbability=maxprobability
    def setProbability(self,value):
        self.probability=value

    def setMaxParent(self, value):
        self.maxParent=value
    def add(self, value,Parent):
        if value>self.maxProbability:
            self.maxProbability=value
            self.maxParent=Parent
        self.probability+=value
class Channel:
    def __init__(self,wbits,sigma):
        self.weight=w_bits
        self.sigma=sigma
    def passData(self,bitStream):
        temp=0.0;
        for i in range(len(w_bits)):
            temp+=int(bitStream[i])*w_bits[i]

        return temp+ np.random.normal(0, self.sigma, 1)[0]

def gaussProbability(x,mean,sigma):
    return  scipy.stats.norm(mean,sigma).pdf(x)

def train(sequence,channel,n):
    transition=np.zeros((2**n,2**n))
    state_probability=np.zeros((2**n))
    state_mean=np.zeros((2**n))
    state_sigma = np.zeros((2 ** n))
    channeled_state=[[] for i in range(2**n)]
    pad=""
    for i in range(0,n-1):
        pad+="0"
    sequence=pad+sequence
    i=0
    while(i+n<=len(sequence)):
        state=int(sequence[i:i+n],2)
        state_probability[state]+=1
        channeled_state[state].append(channel.passData(sequence[i:i+n]))
        if i+n+1<=len(sequence):
            to=int(sequence[i+1:i+n+1],2)
            transition[state][to]+=1
        i+=1
    for i in range(transition.shape[0]):
        transition[i]=transition[i]/np.sum(transition[i])
        if(len(channeled_state[i])!=0):
            state_mean[i]=sum(channeled_state[i])/len(channeled_state[i])
            state_sigma[i]=np.sqrt(np.sum(np.square(channeled_state[i] - state_mean[i]) / len(channeled_state[i])))
            #np.std(channeled_state[1])
    state_probability=state_probability/np.sum(state_probability)
    return transition,state_probability,state_mean,state_sigma


def test(sequence,transition_matrix,state_probability,state_mean,state_sigma,n):
    layers=[]
    initialLayer=[]
    layers.append(initialLayer)
    for i in range(2**n):
        initialLayer.append(state(probability=state_probability[i],Parent=-10))
    current_layer_no=0
    for bit in sequence:
        next_layer=[]
        input_layer=layers[current_layer_no]
        for i in range(2**n):
            next_layer.append(state())
        for i in range(2 ** n):
            for j in range(2**n):
                next_layer[j].add(input_layer[i].probability*transition_matrix[i][j],i)
        for j in range(2**n):
            next_layer[j].probability=next_layer[j].probability*gaussProbability(bit,state_mean[j],state_sigma[j])
        layers.append(next_layer)
        current_layer_no+=1

    max_state=0
    max_probability=-25689555
    for i in range(len(layers[current_layer_no])):
        if max_probability<layers[current_layer_no][i].probability:
            max_state=i
            max_probability=layers[current_layer_no][i].probability
    current_state=max_state
    output=""
    while(current_layer_no>0):
        output="{0:b}".format(current_state)[-1]+output
        current_state=layers[current_layer_no][current_state].maxParent
        current_layer_no-=1
    return  output

def finderror(one,two):
    count=0
    for i in range(len(one)):
        if one[i]!=two[i]:
            count+=1
    return count

def file_reader():

    config=open("config.txt","r")
    n,l=[int(x) for x in config.readline().replace("\n","").split(" ")]
    w_bits=[]
    temp=config.readline().replace("\n","").split(" ")
    for w in temp:
        w_bits.append(float(w))
    sigma=float(config.readline().replace("\n",""))
    sequence_train=open("train.txt").readline()
    sequence_test=open("test.txt").readline()
    return n,w_bits,sigma,sequence_train,sequence_test

def pass_through_channel(sequence,n,channel):
    pad = ""
    channeled_test = []
    i = 0
    for j in range(n - 1):
        pad += "0"
    sequence=pad + sequence
    while (i + n <= len(sequence)):
        channeled_test.append(channel.passData(sequence[i:i + n]))
        i += 1
    return channeled_test


if __name__ == "__main__":
    #np.random.seed(5)
    n,w_bits,sigma,sequence_train,sequence_test=file_reader()
    channel=Channel(w_bits,sigma)
    transition_matrix,state_probability,state_mean,state_sigma=train(sequence_train,channel,n)
    channeled_test=pass_through_channel(sequence_test,n,channel)
    output=test(channeled_test,transition_matrix,state_probability,state_mean,state_sigma,n=n)
    print("Prior Probabilities: ",state_probability.tolist())
    print("Transition Probabilities: ", transition_matrix.tolist())
    print("Observation means: ",state_mean)
    print("Input  : "+sequence_test)
    print("Output : "+output)
    error=finderror(output,sequence_test)
    print("Missmatch: "+str(error))
    print("Error: "+str((error/len(sequence_test))*100))
