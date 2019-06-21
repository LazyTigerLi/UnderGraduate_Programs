import numpy as np
import random


def KMeans(k,data):                 #data包括数据和分组
    dataset = data[0]
    realLabel = data[1]
    length = len(dataset)
    index = random.sample(range(0,length),k)
    centralPoints = np.mat([dataset[i] for i in index])
    clusters = []
    for i in range(k):
        clusters.append([])

    label = [None] * length                      #指示将某条数据归为第几个中心点那一类
    changed = True                              #指示中心点是否发生变化
    while changed:
        changed = False
        for i in range(length):                 #将数据划分到最近的中心点
            distance = np.linalg.norm(centralPoints - dataset[i],axis = 1)
            nearest = np.argsort(distance)[0]
            if nearest != label[i]:
                if label[i] != None:
                    clusters[label[i]].remove(i)
                clusters[nearest].append(i)
                label[i] = nearest
                changed = True
        if changed:                             #计算每个聚类的平均值
            for i in range(k):
                clusterData = np.mat([dataset[j] for j in clusters[i]])
                centralPoints[i] = np.mean(clusterData,axis = 0)
    print(label)


def readDataset(filename):
    dataset = []
    f = open(filename,'r')
    f.readline()
    all_data = f.readlines()
    for line in all_data:
        rawData = line[:-1].split(',')
        data = [float(v) for v in rawData[:-4]]
        label = rawData[-4]
        dataset.append((data,label))
    #print(dataset)
    return [data[0] for data in dataset],[data[1] for data in dataset]

dataset,label = readDataset("Frogs_MFCCs.csv")
KMeans(4,(dataset,label))