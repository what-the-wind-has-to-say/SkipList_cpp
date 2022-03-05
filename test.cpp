#include<bits/stdc++.h>
#include "mySkipList.h"
using namespace std;
int main()
{
    SkipList<int>* list = new SkipList<int>();
    // srand(time(NULL));
    for(int i = 0;i < 20;i++)
    {
        int val = i;
        list->add(new SkipNode<int>(i,&val));
    }
    cout << "删除前：" << endl;
    cout << "highlevel = " << list->highLevel << endl;
    cout << "Size = " << list->size << endl;
    list->printList_std();
    list->deleteNode(4);
    list->deleteNode(8);
    cout << "删除后：" << endl;
    cout << "highlevel = " << list->highLevel << endl;
    cout << "Size = " << list->size << endl;
    list->printList_std();
    return 0;
}

