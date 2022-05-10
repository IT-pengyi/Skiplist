
#include<iostream>
#include "skiplist.h"
#define FILE_PATH "./store/dumpFile"

using  namespace std;


int main() {


    SkipList<int, string> skipList(10);
    
    skipList.insert_element(7, "学习"); 
	skipList.insert_element(8, "Redis");
    skipList.insert_element(9, "跳跃表");
    skipList.search_element(9);
    skipList.search_element(18);
    skipList.delete_element(3);
    skipList.delete_element(7);

    skipList.dump_file();
    skipList.display_list();

}