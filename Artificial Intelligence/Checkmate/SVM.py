from cvxopt import matrix,solvers

def softSVM(trainset,trainlabel,sigma,C):
    