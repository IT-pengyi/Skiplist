
#include<iostream>
#include "skiplist.h"
#define FILE_PATH "./store/dumpFile"

using  namespace std;


int main() {


    SkipList<int, string> skipList(10);
    skipList.insert_element(1, "学"); 
    skipList.insert_element(13, "算法"); 
    

    skipList.insert_element(17, "学习"); 
    skipList.insert_element(18, "Redis");
    skipList.insert_element(19, "跳跃表");
    skipList.insert_element(99, "OK");
    skipList.insert_element(919, "XXX");
    skipList.search_element(9);

    skipList.delete_element(7);

    skipList.dump_file();
    skipList.display_list();

}
