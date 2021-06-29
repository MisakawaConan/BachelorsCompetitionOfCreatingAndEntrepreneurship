import cv2 as cv   #图像特征描述子；图像配准 传统方法 机器学习 神经网络
import numpy as np
import math
from numpy.core.fromnumeric import size
#检查识别到的点的相对位置
def check_position(tuple1:tuple[int,int]):
    sumX=sumY=0
    tuple2:tuple[int,int]=[[0,0],[0,0],[0,0],[0,0]]
    for i in tuple1:
        x,_=i
        _,y=i
        sumX+=x
        sumY+=y
    sumX/=4
    sumY/=4
    for i in tuple1:
        x,_=i
        _,y=i
        if x<sumX :
            if y<sumY:
                tuple2[0]=i
            elif y>sumY:
                tuple2[2]=i
        elif x>sumX:
            if y<sumY:
                tuple2[1]=i
            else:
                tuple2[3]=i
    tuple2
        





if __name__=="__main__":
    srcImg=cv.imread("./newposition/1.jpg")
    (a,b,_)=srcImg.shape
    tuple2=np.float32(([0,0],[0,b],[a,0],[a,b]))#(([0,0],[a,0],[0,b],[a,b]))
    cv.imwrite("dst1.jpg",srcImg)
    dstimg=cv.imread("./dst1.jpg")
    grayImg=cv.cvtColor(srcImg,cv.COLOR_BGR2GRAY)
    grayImg=cv.GaussianBlur(grayImg,(9,9),2,2)
 
    # cv.imshow(" ",grayImg)
    # cv.waitKey(0)
    Circles=cv.HoughCircles(grayImg,cv.HOUGH_GRADIENT, 1, 20,
              param1=100,
            #   param2=35,
                param2=27,
              minRadius=0,
              maxRadius=60#60
              )
    Circles = np.uint16(np.around(Circles))#???
    tuple1:tuple[int,int]=()
    if Circles.size!=0:
        for i in Circles[0,:]:
            cv.circle(dstimg,(i[0],i[1]),i[2],(255,255,255),2)
            cv.circle(dstimg,(i[0],i[1]),2,(255,255,255),3)
            tuple1+=([i[0],i[1]],)
            print(i[0])
    else:
        print("no circle in pic")
    cv.imshow("dstimg",dstimg)
    cv.waitKey(0)
#无法准确预测四个点实际位置,只能希望不是变形太离谱

   
    # cv.waitKey(0)
    tuple1=check_position(tuple1)
    tuple1=np.float32(tuple1)
    print(tuple1)
    M = cv.getPerspectiveTransform(tuple1, tuple2)
    dst = cv.warpPerspective(srcImg, M, (a, b),cv.INTER_LINEAR, cv.BORDER_CONSTANT)
    cv.namedWindow("src",cv.WINDOW_NORMAL)
    cv.imshow("src",srcImg)
    
    cv.namedWindow("dst",cv.WINDOW_NORMAL)
    cv.imshow("dst",dst)
    cv.waitKey(0)