# -*- coding: cp936 -*-
#test.py
import random
import math
import time
points = []
for i in range(10000):
    x = random.randint(-10000,10000)
    y = random.randint(-10000,10000)
    point = str(x) + ' ' + str(y) + '\n'
    points += point
testfile = open('test.txt','w')
testfile.truncate()
testfile.writelines(points)
testfile.close()

testfile = open('test.txt','r')
points = []
'''line = testfile.readline()
temp = line.split(';')
for i in range(len(temp)):
    [x,y] = temp[i].split(',')
    points += [(float(x),float(y))]'''
for line in testfile.readlines():
    [x,y] = line.split()
    points += [(int(x),int(y))]

length = len(points)
nearest_points = []
min_distance = float('inf')

t1 = int(round(time.time() * 1000))
for i in range(length):
	for j in range(i + 1,length):
		d = math.sqrt(pow(points[i][0] - points[j][0],2) + pow(points[i][1] - points[j][1],2))
		if d < min_distance:
			min_distance = d
			nearest_points = [(points[i][0],points[i][1]),(points[j][0],points[j][1])]
t2 = int(round(time.time() * 1000))
print min_distance
print nearest_points
print 'time',t2 - t1,' ms'