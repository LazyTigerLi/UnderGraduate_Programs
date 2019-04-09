import socket
import sys

class dnsRequestMessage:
    def __init__(self,dnsMessage,address,dnsServer,domainName2IP):
        self.quary = dnsMessage[12:]
        self.identification = dnsMessage[0:2]
        self.numberOfQuestions = int.from_bytes(dnsMessage[4:6], byteorder='big', signed=False)
        self.flags = dnsMessage[2:4]
        self.requestMessage = dnsMessage
        self.clientAddress = address
        self.dnsServer = dnsServer
        self.domainName2IP = domainName2IP
        index = 12
        domainName = []
        while dnsMessage[index] != 0:
            length = dnsMessage[index]                          #了解域名是如何存储的很重要
            domainName += dnsMessage[index + 1:index + 1 + length].decode()
            domainName += '.'
            index += length + 1
        self.domainName = ''.join(domainName[:-1])

    def processRequest(self,sock2Client):                       #处理DNS查询报文
        #print(self.domainName)
        if self.domainName in self.domainName2IP:
            ip = self.domainName2IP[self.domainName]
            if ip == '0.0.0.0':
                replyMessage = self.packReplyMessage(ip,True)
            else:
                replyMessage = self.packReplyMessage(ip,False)
            sock2Client.sendto(replyMessage,self.clientAddress)
            #self.analyzeReplyMessage(replyMessage,self.level)
        else:
            sock2DNSServer = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
            time = 1
            while True:
                try:
                    sock2DNSServer.sendto(self.requestMessage,(self.dnsServer,53))
                    sock2DNSServer.settimeout(time)
                    (replyMessage, dnsServer) = sock2DNSServer.recvfrom(4096)
                    #推断此处由于超时一直在等待回复，从而程序无法继续执行
                    #这里一定要绑定端口（服务端绑定特定端口，客户端不需要）
                    #这又导致同一个端口被多个进程占用，所以应该把之前创建的socket作为参数传进来
                    sock2Client.sendto(replyMessage,self.clientAddress)
                    break
                except socket.timeout:
                    time += 1
                    continue
            sock2DNSServer.close()
            return (replyMessage,self.domainName)
            '''这里必须要关闭和dnsserver的连接，否则连接越来越多，会报这个错误：
            ConnectionResetError: [WinError 10054] 远程主机强迫关闭了一个现有的连接。'''

    def packReplyMessage(self,IP,error):                        #生成DNS回复报文
        identification = self.identification
        numberOfQuestions = (self.numberOfQuestions).to_bytes(length = 2,byteorder = 'big',signed = False)
        numberOfAuthorityRRs = (0).to_bytes(length = 2,byteorder = 'big',signed = False)
        numberOfAdditionalRRs = (0).to_bytes(length = 2,byteorder = 'big',signed = False)
        recursionDesired = (int.from_bytes(self.flags, byteorder='big', signed=False)>>8) & 1
        if error:
            flags = (0x8003 + 256 * recursionDesired).to_bytes(length = 2,byteorder = 'big',signed = False)
            numberOfAnswerRRs = (0).to_bytes(length = 2,byteorder = 'big',signed = False)
            quary = self.quary
            reply = identification + flags + numberOfQuestions + numberOfAnswerRRs\
                    + numberOfAuthorityRRs + numberOfAdditionalRRs + quary
            return reply
        else:
            ip = IP.split('.')
            flags = (0x8000 + 256 * recursionDesired).to_bytes(length = 2,byteorder = 'big',signed = False)
            numberOfAnswerRRs = (1).to_bytes(length = 2,byteorder = 'big',signed = False)
            quaryType = (1).to_bytes(length = 2,byteorder = 'big',signed = False)
            quaryClass = (1).to_bytes(length = 2,byteorder = 'big',signed = False)
            #domainName特殊处理,间隔符不是.,而是长度
            nameSlice = self.domainName.split('.')
            for item in nameSlice:
                domainName = (len(item)).to_bytes(length = 1,byteorder = 'big',signed = False) + item.encode()
            domainName += (0).to_bytes(length = 1,byteorder = 'big',signed = False)
            quary = domainName + quaryType + quaryClass
            answerName = (0xc00c).to_bytes(length = 2,byteorder = 'big',signed = False)
            ttl = (100).to_bytes(length = 4,byteorder = 'big',signed = False)
            dataLength = (4).to_bytes(length = 2,byteorder = 'big',signed = False)
            address = int(ip[0]).to_bytes(length = 1,byteorder = 'big',signed = False)\
                    + int(ip[1]).to_bytes(length = 1,byteorder = 'big',signed = False)\
                    + int(ip[2]).to_bytes(length = 1,byteorder = 'big',signed = False)\
                    + int(ip[3]).to_bytes(length = 1,byteorder = 'big',signed = False)
            #address = bytes([int(ip[0]),int(ip[1]),int(ip[2]),int(ip[3])])
            answer = answerName + quaryType + quaryClass + ttl + dataLength + address
            reply = identification + flags + numberOfQuestions + numberOfAnswerRRs\
                    + numberOfAuthorityRRs + numberOfAdditionalRRs + quary + answer
            return reply
