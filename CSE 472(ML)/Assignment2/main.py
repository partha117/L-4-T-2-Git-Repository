from DataGenerate import  generate_sample,plot_cov_ellipse
from GMM import  M_Step,E_Step,loglikelihood
from matplotlib import pyplot as plt
import  numpy as np

mean=np.array([[5,3],[4,10],[.1,.3]])
cov=[[[0.63, 0.12],[0.12, 0.28]],[[0.25, 0.02],[0.02, 0.333]],[[0.7, 0],[0, 0.76]]]
weight=[0.34,0.33,0.33]





data=generate_sample()


p_ij=np.empty(shape=[3,len(data[0])])

prev_liklihood=0
current_liklihood=100
plt.ion()
while(prev_liklihood!=current_liklihood):

    prev_liklihood=current_liklihood
    p_ij=E_Step(data,mean,cov,p_ij,weight)
    M_Step(data,p_ij,mean,cov,weight)
    val=np.sum(weight)
    weight=weight/val
    current_liklihood=loglikelihood(p_ij,weight)
    print(current_liklihood)


    plt.scatter(data[0], data[1])
    for i in range(3):
        plt.plot(mean[i][0], mean[i][1], '+', mew=10, ms=20, color='#f4425f')
        plot_cov_ellipse(cov[i], mean[i])
    plt.draw()
    plt.pause(0.055)


plt.show()


for i in range(3):
    print("--------------Distribution"+str(i+1)+"----------------------")
    print("Mean: "+str(mean[i]))
    print("Covarinace matrix: \n"+str(cov[i]))
    print("Distribution Weight: "+str(weight[i]))
