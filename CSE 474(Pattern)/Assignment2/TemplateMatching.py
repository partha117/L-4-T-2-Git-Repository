import cv2
import math
from matplotlib import  pyplot as plt
import numpy as np
from scipy import ndimage
import sys
import time
class TemplateMatching:

    Match_Method_Exhaustive   = 1
    Match_Method_Logarithmic  = 2
    Match_Method_Hierarchical = 3

    def __init__(self,reference,target):

        self.reference_rgb=cv2.imread(reference)
        self.reference=cv2.cvtColor(self.reference_rgb, cv2.COLOR_BGR2GRAY)
        self.target_rgb=cv2.imread(target)
        self.target= cv2.cvtColor(self.target_rgb, cv2.COLOR_BGR2GRAY)


    def __Exhaustive_Search(self):
        ref_y, ref_x = self.reference.shape
        target_y, target_x = self.target.shape
        time1=time.time()
        best_i = best_j = 0
        min = sys.float_info.max
        for i in range(ref_x - target_x + 1):
            for j in range(ref_y - target_y + 1):
                temp_ref = self.reference[j:j + target_y, i:i + target_x]
                temp_target = self.target[:, :]
                sum_value= (np.sum(np.abs(temp_ref - temp_target)))
                if sum_value < min:
                    best_i, best_j = i, j
                    min = sum_value

        time2=time.time()
        print("Total time: "+str(time2-time1))
        cv2.rectangle(self.reference_rgb,(best_i,best_j),(best_i+target_x,best_j+target_y),(0,0,255),2)
        cv2.imshow("Region",self.reference_rgb)
        cv2.waitKey(0)
        #target = self.reference[best_j:(best_j + target_y), best_i:(best_i + target_x)]
        #plt.imshow(target)
        #plt.show()
    def __getref(self,center_ref_x,center_ref_y,ref=None,target=None):
        if target is None:
            target_y, target_x = self.target.shape
        else:
            target_y, target_x = target.shape
        if ref is None:
            ref_y, ref_x = self.reference.shape
        else:
            ref_y, ref_x = ref.shape
        x,y=int(target_x/2),int(target_y/2)
        x_add=y_add=0
        if x*2!= target_x:
            x_add=1
        if y*2!=target_y:
            y_add=1
        left = center_ref_x - x-x_add
        right = center_ref_x + x
        up = center_ref_y - y-y_add
        down = center_ref_y + y
        temp_ref = self.reference
        if left < 0:
            print("Left")
            temp_ref = cv2.copyMakeBorder(temp_ref, 0, 0, -left, 0, cv2.BORDER_CONSTANT, value=0)
        if right > ref_x:
            print("Right")
            temp_ref = cv2.copyMakeBorder(temp_ref, 0, 0, 0, right - ref_x, cv2.BORDER_CONSTANT, value=0)
        if up < 0:
            print("Up")
            temp_ref = cv2.copyMakeBorder(temp_ref, -up, 0, 0, 0, cv2.BORDER_CONSTANT, value=0)
        if down > ref_y:
            print("Down")
            temp_ref = cv2.copyMakeBorder(temp_ref, 0, down - ref_y, 0, 0, cv2.BORDER_CONSTANT, value=0)

        if up>0:
            up=0
        if left >0:
            left=0
        temp=temp_ref[center_ref_y-y-y_add-up:center_ref_y+y-up,center_ref_x-x-x_add-left:center_ref_x+x-left]
        #plt.imshow(temp)
        #plt.show()
        return temp
    def __plot(self,center_ref_x,center_ref_y):
        target_y, target_x = self.target.shape
        temp_ref = self.reference
        x, y = int(target_x / 2), int(target_y / 2)
        x_add = y_add = 0
        if x != target_x * 2:
            x_add = 1
        if y != target_y * 2:
            y_add = 1
        temp = temp_ref[center_ref_y - y - y_add:center_ref_y + y, center_ref_x - x - x_add:center_ref_x + x]
        plt.imshow(temp)
        plt.show()
    def __Hierarchical_Search(self,level=0,ref=None,target=None):
        if ref is not None and target is not None and (ref.shape[0]<5 or ref.shape[1]<5 or target.shape[0]<3 or target.shape[1] < 3):
            return
        if(level==0):
            time1=time.time()
            ref=self.reference
            cv2.GaussianBlur(ref,(5,5),0)
            ref=ndimage.interpolation.zoom(ref, 0.5, mode='nearest')
            target = self.target
            cv2.GaussianBlur(target, (3, 3), 0)
            target = ndimage.interpolation.zoom(target, 0.5, mode='nearest')
            center_ref_x,center_ref_y=self.__Hierarchical_Search(level+1,ref,target)

            target_y, target_x = self.target.shape
            x, y = int(target_x / 2), int(target_y / 2)
            x_add = y_add = 0
            if x != target_x * 2:
                x_add = 1
            if y != target_y * 2:
                y_add = 1
            time2=time.time()
            print("Total time: " + str(time2 - time1))
            cv2.rectangle(self.reference_rgb, (center_ref_x - x - x_add, center_ref_y - y - y_add),
                          (center_ref_x + x, center_ref_y + y), (0, 0, 255), 2)
            cv2.imshow("Region", self.reference_rgb)
            cv2.waitKey(0)
        else:
            #ref = self.reference
            cv2.GaussianBlur(ref, (5, 5), 0)
            ref = ndimage.interpolation.zoom(ref, 0.5, mode='nearest')
            #target = self.target
            cv2.GaussianBlur(target, (3, 3), 0)
            target = ndimage.interpolation.zoom(target, 0.5, mode='nearest')
            val=self.__Hierarchical_Search(level + 1, ref, target)
            if val is None:
                ref_y, ref_x = ref.shape
                target_y, target_x = target.shape
                best_i = best_j = 0
                min = sys.float_info.max
                for i in range(ref_x - target_x + 1):
                    for j in range(ref_y - target_y + 1):
                        temp_ref = ref[j:j + target_y, i:i + target_x]
                        temp_target = target[:, :]
                        sum_value = (np.sum(np.abs(temp_ref - temp_target)))
                        if sum_value < min:
                            best_i, best_j = i, j
                            min = sum_value
                return (best_i,best_j)
            else:
                center_ref_x=int(ref.shape[1]/2)+2*val[0]
                center_ref_y = int(ref.shape[0] / 2) + 2 * val[1]
                d=1
                min_value = sys.float_info.max
                for i in range(-1, 2, 1):
                    for j in range(-1, 2, 1):
                        # if (i!=0 and j!=0) or first_time:
                        #     first_time=False
                        temp_ref = self.__getref(center_ref_x + i * d, center_ref_y + j * d,ref,target)
                        sum_value = (np.sum(np.abs(temp_ref - target)))
                        if sum_value < min_value:
                            min_value = sum_value
                            p_x, p_y = center_ref_x + i * d, center_ref_y + j * d
                return p_x,p_y
            # cv2.imshow("4d4d",ref)
            # cv2.waitKey(0)


    def __Logarithmic_Search(self):
        time1=time.time()
        ref_y, ref_x = self.reference.shape
        target_y, target_x = self.target.shape


        p=ref_x/2
        d=pow(2,math.ceil(math.log(p,2))-1)
        center_ref_y, center_ref_x = int(ref_y/2), int(ref_x/2)

        first_time=True
        step=0
        while  (d>1) and (step<50):
            min_value=sys.float_info.max
            p_x,p_y=0,0
            step+=1
            for i in range(-1,2,1):
                for j in range(-1,2,1):
                    # if (i!=0 and j!=0) or first_time:
                    #     first_time=False
                        temp_ref=self.__getref(center_ref_x+i*d,center_ref_y+j*d)
                        sum_value=(np.sum(np.abs(temp_ref - self.target)))
                        if sum_value <min_value:
                            min_value=sum_value
                            p_x,p_y=center_ref_x+i*d,center_ref_y+j*d

            #self.__plot(p_x,p_y)
            if p_x==center_ref_x and p_y==center_ref_y:
                p /= 2
                d = pow(2, math.ceil(math.log(p, 2)) - 1)
                center_ref_x, center_ref_y = p_x, p_y
            else:
                center_ref_x, center_ref_y = p_x, p_y
                continue


        x, y = int(target_x / 2), int(target_y / 2)
        x_add = y_add = 0
        if x != target_x * 2:
            x_add = 1
        if y != target_y * 2:
            y_add = 1
        time2=time.time()
        print("Total time: " + str(time2 - time1))
        cv2.rectangle(self.reference_rgb,(center_ref_x - x - x_add,center_ref_y - y - y_add),(center_ref_x + x,center_ref_y + y),(0,0,255),2)
        cv2.imshow("Region",self.reference_rgb)
        cv2.waitKey(0)
    def match(self,Method=Match_Method_Exhaustive):
        if Method==self.Match_Method_Exhaustive:
            self.__Exhaustive_Search()
        elif Method==self.Match_Method_Logarithmic:
            self.__Logarithmic_Search()
        elif Method==self.Match_Method_Hierarchical:
            self.__Hierarchical_Search()



    def get_ref_shape(self):
        return self.reference.shape
    def get_target_shape(self):
        return self.target.shape



