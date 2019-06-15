import numpy as np


def pluarityClassification(labels):
    count = {}
    for label in labels:
        if label in count:
            count[label] += 1
        else:
            count[label] = 1
    #return max(count,key = lambda x: count[x])
    return max(zip(count.values(),count.keys()))[1]

def knn(trainset,trainlabel,testset,testlabel,k):
    result = []
    correct = 0
    for i in range(len(testset)):
        #print(i)
        distance = np.linalg.norm(trainset - testset[i],axis = 1)
        k_nearest_neighbors = np.argsort(distance)[0:k]
        #print(k_nearest_neighbors)
        labels = [trainlabel[index] for index in k_nearest_neighbors]
        result.append(pluarityClassification(labels))
        #print(result[-1])
        if result[-1] == testlabel[i]:
            correct += 1
    print(correct / len(testlabel))
    print(correct)
    print(len(testlabel))
    return result

def readDataset(filename):
    dataset = []
    f = open(filename,'r')
    f.readline()
    all_data = f.readlines()
    for line in all_data:
        rawData = line[:-1].split(',')
        data = [ord(rawData[0]) - ord('a'),ord(rawData[1]) - ord('1'),\
                ord(rawData[2]) - ord('a'),ord(rawData[3]) - ord('1'),\
                ord(rawData[4]) - ord('a'),ord(rawData[5]) - ord('1'),rawData[6]]
        dataset.append(data)
    return np.mat([data[0:6] for data in dataset]),[data[-1] for data in dataset]


(trainset,trainlabel) = readDataset("trainset.csv")
(testset,testlabel) = readDataset("testset.csv")
knn(trainset,trainlabel,testset,testlabel,10)