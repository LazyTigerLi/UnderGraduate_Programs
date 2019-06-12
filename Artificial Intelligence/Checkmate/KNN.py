from queue import PriorityQueue


#应当对训练集和测试集进行预处理，将绝对坐标改成相对位置
def calculate_distance(train_data,test_data):
    return abs(train_data[0] - test_data[0]) + abs(train_data[1]- test_data[1]) + \
        abs(train_data[2] - test_data[2]) + abs(train_data[3] - test_data[3]) + \
        abs(train_data[4] - test_data[4]) + abs(train_data[5] - test_data[5])

def search(train_set,test_data,k):      #返回最近的k个数据点的索引
    distance = PriorityQueue()          #优先队列，保存训练数据与测试数据的距离和训练数据的索引
    for i in range(len(train_set)):
        distance.put((calculate_distance(train_set[i],test_data),i))
    k_nearest_neighbors = []
    for i in range(k):
        k_nearest_neighbors.append(distance.get()[1])
    return k_nearest_neighbors

def pluarityClassification(labels):
    count = {}
    for label in labels:
        if label in count:
            count[label] += 1
        else:
            count[label] = 1
    return max(zip(count.values(),count.keys()))[1]

def knn(trainset,trainlabel,testset,testlabel,k):
    result = []
    for i in range(len(testset)):
        k_nearest_neighbors = search(trainset,testset[i],k)
        labels = [trainlabel[index] for index in k_nearest_neighbors]
        result.append(pluarityClassification(labels))
        #print(result[-1])
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

(trainset,trainlabel) = preprocess(readDataset("trainset.csv"))
(testset,testlabel) = preprocess(readDataset("testset.csv"))
knn(trainset,trainlabel,testset,testlabel,10)