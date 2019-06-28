import numpy as np
import random

def evaluate(N,realClusters,clusters):
    intersect = []
    for i in range(len(clusters)):
        intersect.append([])
    for i in range(len(clusters)):                  #一个矩阵，矩阵元素为每个分组和真实分组的交集的大小
        for realCluster in realClusters:
            intersect[i].append(len(set(clusters[i]).intersection(set(realCluster))))

    purity = 0
    for row in intersect:
        purity += max(row)

    a = 0
    b = 0
    c = 0
    for row in intersect:
        for num in row:
            a += num * (num - 1) / 2
    for row in intersect:
        for i in range(len(row)):
            c += sum(row[i + 1:]) * row[i]
    for j in range(len(intersect[0])):
        for i in range(len(intersect)):
            s = 0
            for k in range(i + 1,len(intersect)):
                s += intersect[k][j]
            b += s * intersect[i][j]
    d = N * (N - 1) / 2
    rand = (a + d) / (a + b + c + d)
    print((a,b,c,d))
    return (purity / N,rand)


def KMeans(k,data):                 #data包括数据和分组
    dataset = data[0]
    realLabel = data[1]
    length = len(dataset)
    index = random.sample(range(0,length),k)
    centralPoints = np.mat([dataset[i] for i in index])
    clusters = []                   #KMeans算法得到的分组
    
    realClusters = []               #真实分组
    feature = dict(zip(list(set(realLabel)),range(len(list(set(realLabel)))))) 
    for i in range(len(feature)):
        realClusters.append([])
    for i in  range(length):
        realClusters[feature[realLabel[i]]].append(i)

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
    print(evaluate(length,realClusters,clusters))


def readDataset(filename):
    dataset = []
    f = open(filename,'r')
    f.readline()
    all_data = f.readlines()
    for line in all_data:
        rawData = line[:-1].split(',')
        data = [float(v) for v in rawData[:-4]]
        label = rawData[-4]                     #这里也可以取其他的真实分组
        dataset.append((data,label))
    #print(dataset)
    return [data[0] for data in dataset],[data[1] for data in dataset]

if __name__ == '__main__':
    dataset,label = readDataset("Frogs_MFCCs.csv")
    KMeans(4,(dataset,label))