import numpy as np
import random

label2Index = {}
classifications = []

def pluarityClassification(labels):
    count = {}
    for label in labels:
        if label in count:
            count[label] += 1
        else:
            count[label] = 1
    return max(count,key = lambda x: count[x])
    #return max(zip(count.values(),count.keys()))[1]


def evaluate(label,realLabel):
    size = len(label)
    correct = 0
    for i in range(size):
        if label[i] == realLabel[i]:
            correct += 1
    accuracy = correct / size

    numOfClassifications = len(classifications)
    F1 = [0] * numOfClassifications
    TP = [0] * numOfClassifications
    FP = [0] * numOfClassifications
    FN = [0] * numOfClassifications
    TN = [0] * numOfClassifications
    for i in range(numOfClassifications):
        for k in range(size):
            if label[k] == i:
                if realLabel[k] == i:
                    TP[i] += 1
                else:
                    FP[i] += 1
            else:
                if realLabel[k] == i:
                    FN[i] += 1
                else:
                    TN[i] += 1
        P = TP[i] / (TP[i] + FP[i])
        R = TP[i] / (TP[i] + FN[i])
        F1[i] = 2 * P * R / (P + R)        
    marcoF1 = sum(F1) / numOfClassifications

    microP = sum(TP) / (sum(TP) + sum(FP))
    microR = sum(TP) / (sum(TP) + sum(FN))
    mircoF1 = 2 * microP *microR / (microP + microR)
    return (accuracy,marcoF1,mircoF1)


def knn(trainset,trainlabel,testset,testlabel,k):
    result = []
    trainset = np.array(trainset)
    testset = np.array(testset)
    for i in range(len(testset)):
        distance = np.linalg.norm(trainset - testset[i],axis = 1)
        k_nearest_neighbors = np.argsort(distance)[0:k]
        labels = [trainlabel[index] for index in k_nearest_neighbors]
        result.append(pluarityClassification(labels))
    print(evaluate(result,testlabel))
    return result

def readDataset(filename):
    dataset = []
    f = open(filename,'r')
    f.readline()
    all_data = f.readlines()
    index = 0
    for line in all_data:
        rawData = line[:-1].split(',')
        data = [ord(rawData[0]) - ord('a'),ord(rawData[1]) - ord('1'),\
                ord(rawData[2]) - ord('a'),ord(rawData[3]) - ord('1'),\
                ord(rawData[4]) - ord('a'),ord(rawData[5]) - ord('1')]
        if rawData[6] not in label2Index:               #将分类用编号代替，因为感觉字符串的开销更高
            label2Index[rawData[6]] = index
            classifications.append(rawData[6])
            index += 1
        data.append(label2Index[rawData[6]])
        dataset.append(data)
    return [data[0:6] for data in dataset],[data[-1] for data in dataset]


def crossValidation(trainset,trainlabel):
    newData = []
    
    data = {}
    size = len(trainset)
    for i in range(size):
        if trainlabel[i] in data:
            data[trainlabel[i]].append(i)
        else:
            data[trainlabel[i]] = []
    for i in range(5):
        newData.append([])
        for label in data:
            length = len(data[label])
            newData[i].extend(data[label][i * length // 5: (i + 1) * length // 5])

    for i in range(5):
        data = []
        label = []
        testdata = []
        testlabel = []
        for j in range(5):
            if j == i:
                testdata = [trainset[k] for k in newData[j]]
                testlabel = [trainlabel[k] for k in newData[j]]
            else:
                data.extend([trainset[k] for k in newData[j]])
                label.extend([trainlabel[k] for k in newData[j]])
        knn(data,label,testdata,testlabel,10)    

if __name__ == '__main__':
    (trainset,trainlabel) = readDataset("trainset.csv")
    (testset,testlabel) = readDataset("testset.csv")
    crossValidation(trainset,trainlabel)
    #knn(trainset,trainlabel,testset,testlabel,10)