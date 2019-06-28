import numpy as np

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
    return [data[0] for data in dataset],[data[1] for data in dataset]


def calculateDistance(cluster1,cluster2):
    d = 0
    for data in cluster1:
        distance = np.linalg.norm(cluster2 - data,axis = 1)
        if max(distance) > d:
            d = max(distance)
    return d   


def findNearestClusters(M):                 #找出距离最近的两个聚类簇
    minColIndex = M.argmin(axis = 1)
    minDistance = M[0][0]
    minRowIndex = 0
    for i in range(len(minColIndex)):
        if M[i][minColIndex[i]] < minDistance:
            minDistance = M[i][minColIndex[i]]
            minRowIndex = i
    #if minRowIndex < minColIndex[minRowIndex]:
    #    return (minRowIndex,minColIndex[minRowIndex])
    #else:
    #    return (minColIndex[minRowIndex],minRowIndex)
    return (minRowIndex,minColIndex[minRowIndex])


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


def HC(data,n_clusters):                    #data包括数据和分组
    dataset = data[0]
    realLabel = data[1]
    size = len(dataset)
    realClusters = []               #真实分组
    feature = dict(zip(list(set(realLabel)),range(len(list(set(realLabel)))))) 
    for i in range(len(feature)):
        realClusters.append([])
    for i in  range(size):
        realClusters[feature[realLabel[i]]].append(i)
    
    clusters = []
    for i in range(size):
        clusters.append([i])
    M = np.zeros((size,size))
    for i in range(size):
        for j in range(i + 1,size):
            print((i,j))
            M[i][j] = calculateDistance(np.mat([dataset[k] for k in clusters[i]]),np.mat([dataset[k] for k in clusters[j]]))
            M[j][i] = M[i][j]
    q = size
    while q > n_clusters:
        (i,j) = findNearestClusters(M)
        clusters[i].extend(clusters[j])     #将两个聚类簇合并
        clusters.pop(j)
        np.delete(M,j,axis = 0)
        np.delete(M,j,axis = 1)             #删除距离矩阵的第j行和第j列
        for k in range(q - 1):
            M[i][k] = calculateDistance(np.mat([dataset[k] for k in clusters[i]]),np.mat([dataset[k] for k in clusters[j]]))
            M[k][i] = M[i][k]
        q = q - 1
        print(q)
    print(evaluate(size,realClusters,clusters))


if __name__ == '__main__':
    dataset,label = readDataset("Frogs_MFCCs.csv")
    HC((dataset,label),4)

#这个算法速度太慢了。。。。。。。。。。。。
    