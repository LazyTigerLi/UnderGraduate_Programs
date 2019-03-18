#include<vector>
#include<string>
#include<fstream>
#include<sstream>
#include<cstring>
#include<map>
#include "LongestWordChain.h"

std::vector<std::pair<int,int>> route[MAX_CELL][MAX_CELL];      //  应该是这里有问题

void DFS(int index,
        std::vector<int>&current_path,
        char specific_tail,unsigned specific_num,
        std::vector<std::pair<int,std::vector<int>>> &result,int depth){

    //判断是否达到要求
    if(specific_num!=0){//指定了-n,从1开始储存
        if(depth==specific_num){
            if(specific_tail!='\0' && specific_tail-'a' != index){//指定了tail, 但是不符合
                return;
            }
            else if(current_path.size()>1){//符合要求, 单词链至少两个单词
                result.push_back(std::make_pair(depth,current_path));
                return;
            }
        }
        else if(depth > specific_num){
            return;
        }
    }
    else{
        if(depth>result[0].first && !(specific_tail!='\0' && specific_tail-'a' != index) && current_path.size()>1){//储存在0
            result[0]=std::make_pair(depth,current_path);
        }
    }

    //寻找下一个节点
    for(int i=0;i<MAX_CELL;i++){
        if(route[index][i].size()>0){
            auto word=route[index][i].back();
            route[index][i].pop_back();
            current_path.push_back(word.second);

            DFS(i,current_path,specific_tail,specific_num,result,depth+word.first);

            route[index][i].push_back(word);
            current_path.pop_back();
        }
    }
}

std::vector<std::pair<int,std::vector<int>>> maxLength(std::vector<std::string>text,std::map<std::string,int>word2id,bool w_or_c,
                                char specific_head,char specific_tail,
                                unsigned specific_num){
    int head[MAX_CELL]={0}; //head[i]有出边

    //填图
    for(auto node:text){
        head[node.front()-'a']=1;
        if(w_or_c){
            route[node.front()-'a'][node.back()-'a'].push_back(std::make_pair(1,word2id[node]));//以word计数
        }
        else{
            route[node.front()-'a'][node.back()-'a'].push_back(std::make_pair(node.size(),word2id[node]));//以char 计数
        }
    }

    //DFS搜索
    std::vector<std::pair<int,std::vector<int>>> result;
    result.push_back(std::make_pair<int,std::vector<int>>(0,{})); //没有-n则只有一个结果，保存在0;有-n则从1开始储存

    if(specific_head!='\0'){//指定了开头
        if(head[specific_head-'a']==1){
            std::vector<int>current_path={};
            DFS(specific_head-'a',current_path,specific_tail,specific_num,result,0);
        }
    }
    else{
        for(int k=0;k<MAX_CELL;k++){//没有指定则遍历
            if(head[k]==1){
                std::vector<int>current_path={};
                DFS(k,current_path,specific_tail,specific_num,result,0);
            }
        }
    }

    return result;
}

/*
    w_or_c -w -c 有且只有一个
    file_name 输入文件
    specific_num -n后的数字,为0则表示为指定
    specific_head -h后的字母，为'\0'则表示未指定
    specific_tail -t后的字母，为'\0'则表示未指定
    输出到solution.txt中
*/

std::string LWC(bool w_or_c, std::string textOriginal, unsigned specific_num, char specific_head, char specific_tail){

    //text处理成vector<string>,仅包含小写字符

    //cout<<textOriginal<<endl;
    std::vector<std::string> text;
    for(auto &c:textOriginal){
        if('a'<=c && c<= 'z')
            continue;
        else if('A'<=c && c<='Z'){
            c=c-'A'+'a';
            continue;
        }
        else
            c=' ';
    }
    size_t pos=0;
    std::string token;
    while((pos=textOriginal.find(' '))!=std::string::npos){
        token=textOriginal.substr(0,pos);
        if(token.length()!=0)
            text.push_back(token);
        textOriginal.erase(0,pos+1);
    }
    if(textOriginal.length()!=0)
        text.push_back(textOriginal);

    std::map<int,std::string> id2word;
    std::map<std::string,int> word2id;
    int word_count=0;
    for(auto w:text){
            id2word.insert(std::make_pair(word_count,w));
            word2id.insert(std::make_pair(w,word_count));
            word_count++;
    }

    //计算
    std::vector<std::pair<int,std::vector<int>>> result=maxLength(text,word2id,w_or_c,specific_head,specific_tail,specific_num);
    //输出
    std::string resultString;
    if(specific_num != 0)
    {
        resultString += std::to_string(result.size() - 1);
        resultString += "\n";
        for(int i = 1; i < result.size(); i++)
        {
            for(auto wordIndex: result[i].second)
                resultString += id2word[wordIndex] + "\n";
            resultString += "\n";
        }
    }
    else
    {
        resultString += std::to_string(result[0].first);
        resultString += "\n";
        for(auto word: result[0].second)
            resultString += id2word[word] + "\n";
    }
    //cout<<resultString;
    for(int i = 0; i < MAX_CELL; i++)
        for(int j = 0; j < MAX_CELL; j++)
            route[i][j].clear();

    return resultString;

    /*ofstream out("solution.txt");
    if(specific_num!=0){
        out<<result.size()-1<<endl;
        for(int i=1;i<result.size();i++){
            for(auto word:result[i].second){
                out<<id2word[word]<<endl;
            }
            out<<endl;
        }
    }
    else{
        out<<result[0].first<<endl;
        for(auto word:result[0].second){
            out<<id2word[word]<<endl;
        }
    }*/
}
