#ifndef _LONGEST_WORD_CHAIN_H_
#define _LONGEST_WORD_CHAIN_H_

#include<vector>
#include<string>
#include<map>

#define MAX_CELL 26

void DFS(int index,
        std::vector<int>&current_path,
        char specific_tail,unsigned specific_num,
        std::vector<std::pair<int,std::vector<int>>> &result,int depth);

std::vector<std::pair<int,std::vector<int>>> maxLength(std::vector<std::string>text,std::map<std::string,int>word2id,bool w_or_c,
                                char specific_head,char specific_tail,
                                unsigned specific_num);

/*
    w_or_c -w -c 有且只有一个
    file_name 输入文件
    specific_num -n后的数字,为0则表示为指定
    specific_head -h后的字母，为'\0'则表示未指定
    specific_tail -t后的字母，为'\0'则表示未指定
    输出到solution.txt中
*/

std::string LWC(bool w_or_c,std::string textOriginal,unsigned specific_num,char specific_head,char specific_tail);
#endif
