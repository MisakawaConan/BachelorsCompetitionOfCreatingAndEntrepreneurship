import cv2 as cv   #图像特征描述子；图像配准 传统方法 机器学习 神经网络
import numpy as np
import math
from numpy.core.fromnumeric import size

if __name__=="__main__":
    cap=cv.VideoCapture(10)
    _,temp=cap.read()
    cv.imshow("temp",temp)
    cv.waitKey(0)
