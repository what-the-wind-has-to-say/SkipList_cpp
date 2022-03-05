#ifndef __mySkiplist__
#define __mySkiplist__
#include<bits/stdc++.h>
using namespace std;
template<class T>
class SkipNode//定义跳表变量类型
{
public:
    int key;//键
    T* value;//值的指针
    SkipNode<T>* right;//右指针
    SkipNode<T>* down;//下指针
    SkipNode(int _key,T* _value):key(_key),value(_value)
    {
        right = NULL;
        down = NULL;
    }
};
template<class T>
class SkipList//跳表类
{
public:
    SkipNode<T>* head;//头节点
    int highLevel;//当前层数
    int MAX_LEVEL = 32;//最大层数
    int size;//数据总数
    SkipList()
    {
        srand(time(NULL));
        head = new SkipNode<T>(INT_MIN,NULL);
        highLevel = 0;
        size = 0;
    }
    SkipNode<T>* search(int key);//查找功能
    void deleteNode(int key);//删除功能
    void add(SkipNode<T>* node);//添加功能
    void printList();//打印整个跳表
    void printList_std();//标准格式打印，支持key < 100
};
//查找，无则返回空，有则返回跳表节点指针
template<typename T>
SkipNode<T>* SkipList<T>::search(int key)
{
    SkipNode<T>* cur = head;
    while(cur != NULL)
    {
        if(cur->key == key)
        {
            return cur;
        }
        else if(cur->right == NULL)//如果右侧没有了，只能下降
        {
            cur = cur->down;
        }
        else if(cur->right->key > key)//需要向下索引
        {
            cur = cur->down;
        }
        else//需要向右索引
        {
            cur = cur->right;
        }
    }
    return NULL;
}
//删除键值为key的跳表节点
template<class T>
void SkipList<T> :: deleteNode(int key)
{
    bool isdelete = false;//判断是否删除了节点
    SkipNode<T>* cur = head;
    while(cur != NULL)
    {
        if(cur->right == NULL)
        {
            cur = cur->down;//向下寻找
        }
        else if(cur->right->key == key)
        {
            isdelete = true;
            cur->right = cur->right->right;//删除节点
            cur = cur->down;//继续向下寻找，删除下层节点
        }
        else if(cur->right->key > key)
        {
            cur = cur->down;//向下寻找
        }
        else
        {
            cur = cur->right;//节点还在右侧
        }
    }
    if(isdelete)size--;//减少一个数据
}
//增加一个节点
template<class T>
void SkipList<T> :: add(SkipNode<T>* node)
{
    int key = node->key;
    //先确定key值是否存在
    SkipNode<T>* findNode = search(key);
    if(findNode != NULL)
    {
        findNode->value = node->value;//更新值
        return;
    }
    size++;//增加了一个数据
    stack<SkipNode<T>*> Nodes;//利用栈储存向下的节点
    SkipNode<T>* cur = head;
    //增加节点需要找到从上到下的前驱节点的路径
    while(cur != NULL)
    {
        if(cur->right == NULL)
        {
            Nodes.push(cur);
            cur = cur->down;
        }
        else if(cur->right->key > key)//不需要取等
        {
            Nodes.push(cur);
            cur = cur->down;
        }
        else
        {
            cur = cur->right;
        } 
    }
    int level = 1;//当前层数，从第一层添加(第一层必须添加)
    SkipNode<T>* downNode = NULL;//底层最开始为空
    while(!Nodes.empty())
    {
        cur = Nodes.top();//抛出待插入的左侧节点
        Nodes.pop();
        SkipNode<T>* newNode = new SkipNode<T>(key,node->value);
        newNode->down = downNode;//竖直方向处理
        downNode = newNode;//标记新节点
        if(cur->right == NULL)//水平方向处理
        {
            cur->right = newNode;//直接插入
        }
        else
        {
            newNode->right = cur->right;
            cur->right = newNode;
        }
        //考虑是否向上搭建索引
        if(level > MAX_LEVEL)break;//不能超过最大层数
        //利用随机数决定是否向上搭建索引
        int rand_num = rand() % 101;
        if(rand_num > 50)break;
        level++;
        if(level > highLevel)//向上新建索引需要重定头节点
        {
            highLevel = level;
            SkipNode<T>* new_head = new SkipNode<T>(INT_MIN,NULL);
            new_head->down = head;
            head = new_head;
            Nodes.push(head);//下一次抛出head
        }
    }
}
//打印整个跳表
template<class T>
void SkipList<T> :: printList()
{
    //将头节点指针全部放入数组，按行打印
    vector<SkipNode<T>*>vec(MAX_LEVEL);
    SkipNode<T>* cur = head;
    int index = 0;
    while(cur != NULL)
    {
        vec[index] = cur;
        index++;
        cur = cur->down;
    }
    //按行打印
    for(int i = 0;i < index;i++)
    {
        cout << index - i << " head -> ";
        cur = vec[i]->right;
        while(cur != NULL)
        {
            cout << cur->key << " -> ";
            cur = cur->right;
        }
        cout << endl;
    }
}
//打印标准格式的跳表

template<class T>
void SkipList<T> :: printList_std()
{
    vector<vector<int> >keys(highLevel,vector<int>(size,-1));
    //将头节点指针全部放入数组
    vector<SkipNode<T>*>vec(MAX_LEVEL);
    SkipNode<T>* cur = head;
    int index = 0;
    while(cur != NULL)
    {
        vec[index] = cur;
        index++;
        cur = cur->down;
    }
    //由于可能存在删除后key值不连续的情况，需要建议一个下标转换关系表
    map<int,int>key_to_index;
    cur = vec[index - 1]->right;//指向头节点后的第一个数据
    index = 0;
    while(cur != NULL)
    {
        key_to_index[cur->key] = index;
        index++;
        cur = cur->right;
    }
    for(int i = 0;i < highLevel;i++)
    {
        cur = vec[i]->right;
        while(cur != NULL)
        {
            keys[i][key_to_index[cur->key]] = cur->key;//标记
            cur = cur->right;
        }
    }
    //标准格式打印
    for(int i = 0;i < highLevel;i++)
    {
        cout << highLevel - i << "head -> ";
        for(int j = 0;j <size;j++)
        {
            string str;
            if(keys[i][j] < 0)
            {
                //由于最后一行不是索引，可以用于判断需要添加多少空格
                if(keys[highLevel - 1][j] < 10)str = "    ";
                else if(keys[highLevel - 1][j]< 100)str = "     ";
            }
            else
            {
                cout << keys[i][j];
                str = "-> ";
            }
            cout << str;
        }
        cout << endl;
    }
}
#endif