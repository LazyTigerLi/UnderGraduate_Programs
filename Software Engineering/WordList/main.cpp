#include <string>
#include <iostream>
#include <list>
#include <vector>
#include <set>

struct VNode
{
    std::list<int> ArcList;
    const char *word;
    int length;
    bool visited;
};

struct graph
{
    int vexnum,arcnum;
    std::vector<VNode *> vertices;
};

void createGraph(std::string s,graph &gra)
{
    gra.vexnum = 0;
    gra.arcnum = 0;
    std::list<int> tailLetter[26];       //分别以各个字母结尾的单词索引
    std::list<int> headLetter[26];       //分别以各个字母开头的单词索引
    std::set<std::string> words;
    int begin = 0;
    bool isletter = false;          //前一个字符是否为字母
    for(int i = 0; i < s.length(); i++)
    {
        if(!((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z')))
        {
            isletter = false;
            continue;
        }
        if(!isletter && ((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z')))
        {
            begin = i;
            isletter = true;
        }
        if(s[i] >= 'A' && s[i] <= 'Z')s[i] -= 'A' - 'a';        //转为小写
        if(!((s[i + 1] >= 'a' && s[i + 1] <= 'z') || (s[i + 1] >= 'A' && s[i + 1] <= 'Z')))
        {
            std::string newWord(s.substr(begin,i - begin + 1));
            if(words.find(newWord) == words.end())
            {
                words.insert(newWord);

                headLetter[s[begin] - 'a'].push_back(gra.vexnum);
                tailLetter[s[i] - 'a'].push_back(gra.vexnum);

                VNode *newNode = new VNode;
                newNode->word = new char[i - begin + 2];
                newNode->word = newWord.data();
                newNode->length = i - begin + 1;
                newNode->visited = false;
                gra.vertices.push_back(newNode);
                gra.vexnum++;
            }
        }
    }
    for(int i = 0; i < 26; i++)
        for(std::list<int>::iterator it1 = tailLetter[i].begin(); it1 != tailLetter[i].end(); it1++)
        {
            for(std::list<int>::iterator it2 = headLetter[i].begin(); it2 != headLetter[i].end(); it2++)
            {
                if(*it1 != *it2)
                {
                    gra.vertices[*it1]->ArcList.push_back(*it2);
                    gra.arcnum++;
                }
            }
        }
    //以上为建立邻接表
}



int main()
{
    std::string s;
    getline(std::cin,s);
    graph gra;
    createGraph(s,gra);

    for(int i = 0; i < gra.vexnum; i++)
    {
        std::cout<<gra.vertices[i]->word<<'\t';
        for(std::list<int>::iterator it = gra.vertices[i]->ArcList.begin();
            it != gra.vertices[i]->ArcList.end(); it++)
            std::cout<<gra.vertices[*it]->word<<'\t';
        std::cout<<std::endl;
    }

    return 0;
}
