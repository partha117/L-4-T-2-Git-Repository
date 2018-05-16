import  numpy as np
from scipy.stats import  multivariate_normal
import  math


def E_Step(data,mean,cov,p_ij,weight):
    for i in range(3):
        temp=multivariate_normal.pdf(data.T,mean[i],cov[i])
        p_ij[i]=temp*weight[i]

    for j in range(len(p_ij[0])):
        sum=0
        for i in range(3):
            sum+=p_ij[i][j]
        for i in range(3):
            p_ij[i][j]=p_ij[i][j]/sum

    return p_ij
def loglikelihood(p_ij,weight):
    sum=0
    val = np.sum(p_ij)

    temp_p_ij=(p_ij*val).T
    for i in range(0,len(p_ij[0])):
        temp=np.sum(temp_p_ij[i]*weight)
        if temp !=0:

            sum=sum+math.log(temp)
    return sum
def M_Step(data,p_ij,mean,cov,weight):


    for i in range(3):
        all_p_ij = np.sum(p_ij[i])
        temp=np.dot(data.copy(),p_ij[i])
        mean[i]=[temp[0],temp[1]]/all_p_ij
        sub=data.copy()-mean[i].reshape(2,1)
        temp=p_ij[i]*sub
        cov[i]=np.dot(temp,sub.T)
        cov[i]=cov[i]/all_p_ij
        weight[i] =all_p_ij / len(data[0])












