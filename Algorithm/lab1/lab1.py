# -*- coding: cp936 -*-
#lab1.py
import math
import time
def quickSort(points,p,r,which):
    if p < r:
        q = partition(points,p,r,which)
        quickSort(points,p,q - 1,which)
        quickSort(points,q + 1,r,which)

def partition(points,p,r,which):
    if which == 'x':
        x = points[r][0]
        i = p - 1
        for j in range(p,r):
            if points[j][0] <= x:
                i += 1
                temp = points[i]
                points[i] = points[j]
                points[j] = temp
        temp = points[i + 1]
        points[i + 1] = points[r]
        points[r] = temp
        return i + 1
    else:
        x = points[r][1]
        i = p - 1
        for j in range(p,r):
            if points[j][1] <= x:
                i += 1
                temp = points[i]
                points[i] = points[j]
                points[j] = temp
        temp = points[i + 1]
        points[i + 1] = points[r]
        points[r] = temp
        return i + 1

def findNearestPoints(points,x,y):
    length = len(points)
    if length == 3:
        d1 = math.sqrt(pow(points[0][0] - points[1][0],2) + pow(points[0][1] - points[1][1],2))
        d2 = math.sqrt(pow(points[0][0] - points[2][0],2) + pow(points[0][1] - points[2][1],2))
        d3 = math.sqrt(pow(points[1][0] - points[2][0],2) + pow(points[1][1] - points[2][1],2))
        if d1 <= d2 and d1 <= d3:
        	return (d1,((points[0][0],points[0][1]),(points[1][0],points[1][1])))
        if d2 <= d1 and d2 <= d3:
        	return (d2,((points[0][0],points[0][1]),(points[2][0],points[2][1])))
        if d3 <= d1 and d3 <= d2:
        	return (d3,((points[1][0],points[1][1]),(points[2][0],points[2][1])))
    elif length == 2:
        return (math.sqrt(pow(points[0][0] - points[1][0],2) + pow(points[0][1] - points[1][1],2)),
        		((points[0][0],points[0][1]),(points[1][0],points[1][1])))
    else:
        l = points[(length + 1)/2][0]
        points_left = points[0 :(length + 1)/2]
        points_right = points[(length + 1)/2 : length]
        x_left = points_left
        x_right = points_right
        y_left = []
        y_right = []
        for i in range(len(y)):
            if y[i] in points_left:
                y_left.append(y[i])
            else:
                y_right.append(y[i])
        (d1,points_pair1) = findNearestPoints(points_left,x_left,y_left)
        (d2,points_pair2) = findNearestPoints(points_right,x_right,y_right)
        if d1 <= d2:
        	d = d1
        	points_pair = points_pair1
        else:
        	d = d2
        	points_pair = points_pair2
        	
        y_middle = []
        for i in range(len(y)):
            if abs(y[i][0] - l) <= d:
            	y_middle.append(y[i])
        if len(y_middle) > 1:
            for i in range(len(y_middle)):
                for j in range(i + 1,i + 8):
                    if j >= len(y_middle):
                        break
                    d3 = math.sqrt(pow(y_middle[i][0] - y_middle[j][0],2) + pow(y_middle[i][1] - y_middle[j][1],2))
                    if d3 < d:
                        d = d3
                        points_pair = ((y_middle[i][0],y_middle[i][1]),(y_middle[j][0],y_middle[j][1]))
        return (d,points_pair)

#由于数据输入格式的不同，程序可能无法运行
testfile = open('test.txt','r')
points = []
line = testfile.readline()
temp = line.split(';')
for i in range(len(temp)):
    [x,y] = temp[i].split(',')
    points += [(float(x),float(y))]
'''for line in testfile.readlines():
    [x,y] = line.split()
    points += [(int(x),int(y))]''' 

length = len(points)

t1 = int(round(time.time() * 1000))
quickSort(points,0,length - 1,'y')
y = points[:]
quickSort(points,0,length - 1,'x')
x = points

(d,nearest_points) = findNearestPoints(points,x,y)
print d
print nearest_points
t2 = int(round(time.time() * 1000))
print 'time:',t2 - t1,' ms'
