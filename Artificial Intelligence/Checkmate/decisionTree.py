import math

attrValue = (-7,-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6,7)

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
        childSets[data[k] + 7].append(data)
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

def preprocess(dataset):            #对数据集进行预处理，这里的dataset是包含标签的
    for data in dataset:
        attr0 = data[0]
        attr1 = data[1]
        data[0] -= data[2]          #白手国王和白手车的距离
        data[1] -= data[3]
        data[2] -= data[4]          #白手车和黑手国王的距离
        data[3] -= data[5]
        data[4] = attr0 - data[4]   #白手国王和黑手国王的距离
        data[5] = attr1 - data[5]
    return [data[0:6] for data in dataset],[data[-1] for data in dataset]

def pluarityClassification(labels):
    count = {}
    for label in labels:
        if label in count:
            count[label] += 1
        else:
            count[label] = 1
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
        childSets[data[feature] + 7].append(data)
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

    correct = 0
    result = []
    for i in range(len(testset)):
        tempTree = tree
        while isinstance(tempTree,dict):
            feature = list(tempTree.keys())[0]
            tempTree = tempTree[feature]
            #print(testset[i][feature])
            #print(testset[i][feature] in tempTree)
            tempTree = tempTree[testset[i][feature]]
        
        if tempTree == testlabel[i]:
            correct += 1
        result.append(tempTree)
        print(result[-1])
    print(correct / len(testset))
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
    return dataset


(trainset,trainlabel) = preprocess(readDataset("trainset.csv"))
(testset,testlabel) = preprocess(readDataset("testset.csv"))
createTree(trainset,trainlabel,testset,testlabel)
