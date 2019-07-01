from cvxopt import matrix,solvers
import numpy as np

def calculateK(dataset,sigma):            #dataset每一行为一条数据，numpy矩阵
    if sigma == 0:
        np.savetxt("kernel_"+str(sigma),dataset.dot(dataset.T), fmt="%d", delimiter=",")
        return
    else:
        k = np.zeros((len(dataset),len(dataset)),'float32')
        for i in range(len(dataset)):
            print(i)
            k[i] = (np.linalg.norm(dataset - dataset[i],axis = 1))
        k = -(k / sigma)**2
        k = np.exp(k)
        np.savetxt("kernel_"+str(sigma),k.T, fmt="%f", delimiter=",")

#def softSVM(trainset,trainlabel,sigma,C):       #trainLabel经过处理，为1和-1
#    n = len(trainlabel)
    '''P = np.zeros((n,n))
    for i in range(n):
        for j in range(n):
            print(i,j)
            P[i][j] = trainlabel[i] * trainlabel[j] * calculateK(trainset[i],trainset[j],sigma)
    q = np.ones((1,len(trainset)))
    q = -q
    G = np.eye(n)
    G = np.vstack((G,-G))
    G = G.T
    h = np.zeros((1,len(trainset)))
    h = np.hstack(h + C,h)
    A = np.mat(trainlabel)
    b = np.mat(0)

    solution = solvers.qp(P,q,G,h,A,b)
    print(solution['x'])'''
    '''
    k = calculateK(trainset,sigma)
    y1 = np.mat(trainlabel,'bool_')
    y = y1.T
    y = y.dot(y1)
    k = np.multiply(y,k)
    gc.collect()
    q = [-1.0] * n
    G = np.eye(n)
    G = np.vstack((G,-G))
    G = G.T
    h = [0.0] * n + [C] * n
    A = trainlabel
    b = 0.0
    print("1234")
    solution = solvers.qp(k,q,G,h,A,b)

    print(solution['x'])

def multiClassSVM(trainset,trainlabel,testset,testlabel):
    labels = set(trainlabel)
    for label in labels:
        processedLabel = []
        for l in trainlabel:
            if l == label:
                processedLabel.append(1)
            else:
                processedLabel.append(-1)
        softSVM(trainset,processedLabel,0,1)
        break
'''

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
calculateK(trainset,0)
calculateK(trainset,1)
calculateK(trainset,10)
calculateK(trainset,0.1 )