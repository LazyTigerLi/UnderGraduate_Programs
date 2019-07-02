import math
import plotTree

label2Index = {}
classifications = []

attrValue = (0,1,2,3,4,5,6,7)

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

def calculateH(dataset):            #计算熵,这里的dataset是包含标签的
    count = {}
    length = len(dataset)
    for i in  range(length):
        label = dataset[i][6]       #6个属性，最后一个是标签
        if label in count:
            count[label] += 1
        else:
            count[label] = 1
    h = 0
    for key in count.keys():
        p = count[key] / length
        h -= p * math.log2(p)
    return h

def calculateIG(dataset,k):          #计算信息增益，k表明取第几个属性来划分，这里的dataset是包含标签的
    IG = calculateH(dataset)
    length = len(dataset)
    childSets = []
    for i in range(len(attrValue)):
        childSets.append([])        #注意这里不能用childSets = [[]] * 15的创建方式，因为所有[]实际是同一个对象
    for data in dataset:            #划分数据集
        childSets[data[k]].append(data)
    for childSet in childSets:
        IG -= len(childSet) * calculateH(childSet) / length
    return IG

def chooseBestFeature(dataset,attrLeft):         #这里的数据集是带标签的
    IG = -1
    which = 0                           #which表明选择哪一个属性
    for i in attrLeft:                  #总共有6个属性
        ig = calculateIG(dataset,i)
        if ig > IG:
            IG = ig
            which = i
        #print(ig)
    return which

def pluarityClassification(labels):
    count = {}
    for label in labels:
        if label in count:
            count[label] += 1
        else:
            count[label] = 1
    #return max(count,key = lambda x: count[x])
    return max(zip(count.values(),count.keys()))[1]

def create(trainset,trainlabel,attrLeft):        #构造决策树，这里的trainset包含trainlabel
    if trainlabel.count(trainlabel[0]) == len(trainlabel):
        return trainlabel[0]            #属于同一类型
    if len(attrLeft) == 0:           #无属性可用
        return pluarityClassification(trainlabel)
    feature = chooseBestFeature(trainset,attrLeft)
    #print(attrLeft)
    #print(feature)
    attrLeft.remove(feature)
    tree = {feature:{}}    
    childSets = []
    for i in range(len(attrValue)):     #划分数据集
        childSets.append([])
    for data in trainset:            
        childSets[data[feature]].append(data)
    for i in range(len(attrValue)):
        if len(childSets[i]) == 0:          #子数据集为空集
            tree[feature][attrValue[i]] = pluarityClassification(trainlabel)
        else:
            attr = attrLeft[:]          #这里必须要复制，否则一直是对一个对象进行操作
            tree[feature][attrValue[i]] = create(childSets[i],[data[-1] for data in childSets[i]],attr)
    return tree
    

def createTree(trainset,trainlabel,testset,testlabel):      #这里的trainset不包含trainlabel
    dataset = trainset[:]
    for i in range(len(trainset)):
        dataset[i].append(trainlabel[i])
    tree = create(dataset,trainlabel,[0,1,2,3,4,5])

    result = []
    for i in range(len(testset)):
        tempTree = tree
        while isinstance(tempTree,dict):
            feature = list(tempTree.keys())[0]
            tempTree = tempTree[feature]
            #print(testset[i][feature])
            #print(testset[i][feature] in tempTree)
            tempTree = tempTree[testset[i][feature]]
        result.append(tempTree)
        #print(result[-1])
    print(evaluate(result,testlabel))
    return tree


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


if __name__ == '__main__':
    (trainset,trainlabel) = readDataset("trainset.csv")
    (testset,testlabel) = readDataset("testset.csv")
    tree = createTree(trainset,trainlabel,testset,testlabel)
    #print(tree)
    #plotTree.createPlot(tree)
