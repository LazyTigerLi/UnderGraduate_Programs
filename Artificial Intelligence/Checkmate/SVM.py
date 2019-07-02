from cvxopt import matrix,solvers
import numpy as np
import random

label2Index = {}
classifications = []


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
        if TP[i] + FP[i] == 0:
            P = 0
        else:
            P = TP[i] / (TP[i] + FP[i])
        R = TP[i] / (TP[i] + FN[i])
        if P + R == 0:
            F1[i] = 0
        else:
            F1[i] = 2 * P * R / (P + R)        
    marcoF1 = sum(F1) / numOfClassifications

    microP = sum(TP) / (sum(TP) + sum(FP))
    microR = sum(TP) / (sum(TP) + sum(FN))
    mircoF1 = 2 * microP *microR / (microP + microR)
    return (accuracy,marcoF1,mircoF1)

def calculateK(dataset,sigma):            #dataset每一行为一条数据，numpy矩阵
    if sigma == 0:
        return (dataset.dot(dataset.T)).astype(np.float64)
    else:
        k = np.zeros((len(dataset),len(dataset)))
        for i in range(len(dataset)):
            k[i] = (np.linalg.norm(dataset - dataset[i],axis = 1))
        k = -(k / sigma)**2
        k = np.exp(k)
        return k.T

def softSVM(trainset,trainlabel,sigma,C):       #trainLabel经过处理，为1和-1
    n = len(trainlabel)
    print("hahaha")
    k = calculateK(trainset,sigma)
    P = matrix(np.outer(trainlabel,trainlabel) * k)
    q = matrix(np.ones(n) * (-1))
    A = matrix(trainlabel,(1,n))
    b = matrix(0.0)
    G = None
    h = None
    if C == 0:
        G = matrix(np.identity(n) * (-1))
        h = matrix(np.zeros(n))
    else:
        G = matrix(np.vstack((np.identity(n),-np.identity(n))))
        h = matrix(np.hstack((np.ones(n) * C,np.zeros(n))))
    solution = solvers.qp(P,q,G,h,A,b)
    w = np.array([0.0] * len(trainset[0]))
    b = 0
    alpha = list(solution['x'])
    for i in range(len(alpha)):
        if abs(alpha[i]) > 1e-9:
            w += alpha[i] * trainlabel[i] * trainset[i]
        else:
            alpha[i] = 0
    for i in range(len(alpha)):
        if alpha[i] != 0:
            b = trainlabel[i] - np.dot(w,trainset[i])
            break
    return w,b

def multiClassSVM(trainset,trainlabel,testset,testlabel):
    wb = []                             #参数w和b的列表
    #print(labels)
    for label in classifications:
        processedLabel = []
        for l in trainlabel:
            if l == label2Index[label]:
                processedLabel.append(1.0)
            else:
                processedLabel.append(-1.0)
        #print(processedLabel)
        wb.append(softSVM(trainset,processedLabel,10,2))
    #print(wb)
    #print("finished")
    result = []
    for data in testset:
        v = []
        for arg in wb:
            v.append(np.dot(data,arg[0]) + arg[1])
        #print(v)
        result.append(np.argmax(np.array(v)))
    print(result)
    print(evaluate(result,testlabel))


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


def preprocessData(trainset,trainlabel):
    data = {}
    size = len(trainset)
    index = []
    for i in range(size):
        if trainlabel[i] in data:
            data[trainlabel[i]].append(i)
        else:
            data[trainlabel[i]] = []
    for label in data:
        index.extend(random.sample(data[label],len(data[label]) * 1000 // size))
    return np.array([trainset[i] for i in index]),[trainlabel[i] for i in index]


if __name__ == "__main__":
    (trainset,trainlabel) = readDataset("trainset.csv")
    (newTrainset,newTrainlabel) = preprocessData(trainset,trainlabel)
    (testset,testlabel) = readDataset("testset.csv")
    multiClassSVM(newTrainset,newTrainlabel,testset,testlabel)