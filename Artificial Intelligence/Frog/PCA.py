import numpy as np
import KMeans
from matplotlib import pyplot as plt

def PCA(data,threshold):
    #center = np.mean(data,axis = 0)     #对所有样本进行中心化
    #centralizedData = data - center
    covMat = np.cov(data,rowvar = False)        #协方差矩阵
    eigenValue,featureVector = np.linalg.eig(covMat)
    eigenValueSum = np.sum(eigenValue)
    
    tempSum = 0
    index = np.argsort(eigenValue)
    featureNum = 0
    for i in reversed(index):
        tempSum += eigenValue[i]
        featureNum += 1
        if tempSum / eigenValueSum >= threshold:
            break
    w = np.mat([featureVector[i] for i in reversed(index[-featureNum:])])     #投影矩阵
    w = w.T
    return np.dot(data,w)


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
    return np.mat([data[0] for data in dataset]),[data[1] for data in dataset]

def display(dataset):
    xValues = [data[0] for data in dataset]
    yValues = [data[1] for data in dataset]
    plt.scatter(xValues,yValues,s = 2)
    plt.title('PCA')
    plt.show()


if __name__ == '__main__':
    dataset,label = readDataset("Frogs_MFCCs.csv")
    #KMeans.KMeans(4,(dataset.tolist(),label))
    KMeans.KMeans(4,(newData,label))
    newData = PCA(dataset,0.6).tolist()
    print(len(newData[0]))
    if len(newData[0]) == 2:
        display(newData)