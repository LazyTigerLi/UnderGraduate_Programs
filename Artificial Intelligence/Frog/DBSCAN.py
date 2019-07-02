import numpy as np

def evaluate(N,realClusters,clusters):
    intersect = []
    for i in range(len(clusters)):
        intersect.append([])
    for i in range(len(clusters)):                  #一个矩阵，矩阵元素为每个分组和真实分组的交集的大小
        for realCluster in realClusters:
            intersect[i].append(len(clusters[i].intersection(realCluster)))

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


def neighborhood(dataset,data,eps):
    distance = np.linalg.norm(dataset - data,axis = 1)
    neighbor = []
    for i in range(len(distance)):
        if distance[i] <= eps:
            neighbor.append(i)
    return neighbor


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


def DBSCAN(data,eps,minPts):
    dataset = data[0]
    realLabel = data[1]
    size = len(dataset)
    realClusters = []               #真实分组
    feature = dict(zip(list(set(realLabel)),range(len(list(set(realLabel)))))) 
    for i in range(len(feature)):
        realClusters.append(set())
    for i in  range(size):
        realClusters[feature[realLabel[i]]].add(i)
    
    core = set()
    N = []
    clusters = []
    for i in range(size):
        #print(i)
        N.append(neighborhood(np.mat(dataset),np.mat(dataset[i]),eps))
        if len(N[i]) >= minPts:
            core.add(i)
    numOfClusters = 0
    unvisited = set(range(size))
    while len(core) != 0:
        #print(len(core))
        unvisitedOld = unvisited.copy()
        q = []
        coreObject = core.pop()
        q.append(coreObject)
        unvisited.remove(coreObject)
        while len(q) != 0:
            index = q.pop(0)
            if len(N[index]) >= minPts:
                delta = set(N[index]).intersection(unvisited)
                q.extend(delta)
                unvisited = unvisited - delta
        numOfClusters += 1
        clusters.append(unvisitedOld - unvisited)
        core = core - clusters[-1]

    print(evaluate(size,realClusters,clusters))


if __name__ == '__main__':
    dataset,label = readDataset("Frogs_MFCCs.csv")
    DBSCAN((dataset,label),2,100)