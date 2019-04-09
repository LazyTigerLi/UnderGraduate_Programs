import sys
import socket
import dnsRequestMessage
import socketserver
import _thread
import queue
import time

argcs = len(sys.argv)
dnsServer = "202.106.0.20"                              #这里一开始在202前面加了个空格...
iniFile = "dnsrelay.txt"
domainName2IP = {}
level = 1
q = queue.Queue()

def configFile(filename):                               #设置配置文件
    try:
        f = open(filename,'r')
        for line in f:
            if line[-1] == '\n':
                line = line[:-1]
            (IP,domainName) = line.split(' ')
            #一个费了不少时间的错误，从配置文件中读取后行末是有换行符的
            #print(domainName + ' ' + IP)
            domainName2IP[domainName] = IP
        return True
    except IOError:                                     #各种异常处理很重要，这是程序健壮性的体现
        print("The configuration file does not exist!")
        return False
    except ValueError:
        print("Invalid domain name or IP address!")
        return False

'''一开始是将analyze函数放在dnsRequestMessage模块中的，但是udphreadingserver是并行的，
会导致analyze函数的输出顺序错乱，故只能放在外面统一处理'''
def analyzeReplyMessage(replyMessage,clientAddress,domainName):
        ip = []
        numberOfQuestions = int.from_bytes(replyMessage[4:6], byteorder='big', signed=False)
        numberOfAnswerRRs = int.from_bytes(replyMessage[6:8], byteorder='big', signed=False)
        i = numberOfQuestions
        index = 12
        while i > 0:
            while replyMessage[index] != 0:
                index = index + 1
            index = index + 5                           #这里在跳过query的时候，一开始index = index + 4,找了半天错误
            i = i - 1
        i = numberOfAnswerRRs
        while i > 0:
            index = index + 2
            dataLength = int.from_bytes(replyMessage[index + 8:index + 10], byteorder='big', signed=False)
            type = int.from_bytes(replyMessage[index:index + 2], byteorder='big', signed=False)
            replyClass = int.from_bytes(replyMessage[index + 2:index + 4], byteorder='big', signed=False)
            if type == 1 and replyClass == 1:
                index += 10                             #这里一开始是index+8，又找了好久
                ip.append(str(replyMessage[index]) + '.' + str(replyMessage[index + 1]) + '.'
                            + str(replyMessage[index + 2]) + '.' + str(replyMessage[index + 3]))
                index += dataLength
            else:
                index = index + 10 + dataLength
            i = i - 1
        if level == 3:
            identification = int.from_bytes(replyMessage[0:2], byteorder='big', signed=False)
            flags = replyMessage[2:4]
            numberOfAuthorityRRs = int.from_bytes(replyMessage[8:10], byteorder='big', signed=False)
            numberOfAdditionalRRs = int.from_bytes(replyMessage[10:12], byteorder='big', signed=False)
            print("identification\t",identification)
            print("flags\t",flags)
            print("questions\t",numberOfQuestions)
            print("answerRRs\t",numberOfAnswerRRs)
            print("authorityRRs\t",numberOfAuthorityRRs)
            print("additionalRRs\t",numberOfAdditionalRRs)
            if numberOfAnswerRRs != 0:
                print("type\t",type)
                print("class\t",replyClass)
            print("client address\t",clientAddress)
            print("server address\t",(socket.gethostbyname(socket.gethostname()),53))
            print("domain name\t",domainName)
            for item in ip:
                print(item)
            print()
        elif level == 2:
            print("time\t",time.strftime("%Y-%m-%d %H:%M:%S",time.localtime(time.time())))
            identification = int.from_bytes(replyMessage[0:2], byteorder='big', signed=False)
            print("identification\t",identification)
            print("client address\t",clientAddress)
            print("domain name\t",domainName)
            print()
        else:
            print("domain name\t",domainName)
            for item in ip:
                print(item)
            print()

def debug():
    while True:
        (replyMessage,domainName,clientAddress) = q.get()
        analyzeReplyMessage(replyMessage,clientAddress,domainName)

class requestHandler(socketserver.BaseRequestHandler):
    def handle(self):
        data = self.request[0]
        sock = self.request[1]
        request = dnsRequestMessage.dnsRequestMessage(data,self.client_address,dnsServer,domainName2IP)
        (replyMessage,domainName) = request.processRequest(sock)
        q.put((replyMessage,domainName,self.client_address))

def serve():
    _thread.start_new_thread(debug,())
    server_address = ('localhost', 53)                  #dns使用端口53,这里一定要设置
    server = socketserver.ThreadingUDPServer(server_address,requestHandler)
    server.serve_forever()

if argcs == 1:                                          #调试级别1
    if configFile("dnsrelay.txt"):
        level = 1
        serve()
elif argcs == 4:                                        #调试级别2
    if sys.argv[1] != '-d':
        print("The arguments are incorrect!")
    else:
        if configFile(sys.argv[3]):
            dnsServer = sys.argv[2]
            level = 2
            serve()
else:                                                   #调试级别3
    if sys.argv[1] != '-dd':
        print("The arguments are incorrect!")
    else:
        if configFile("dnsrelay.txt"):
            dnsServer = sys.argv[2]
            level = 3
            serve()