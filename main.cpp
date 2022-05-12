
#include<iostream>
#include "skiplist.h"
#define FILE_PATH "./store/dumpFile"

using  namespace std;


int main() {


    SkipList<int, string> skipList(10);
    skipList.insert_element(1, "学"); 
	skipList.insert_element(13, "算法"); 
	skipList.insert_element(7, "认准"); 
	skipList.insert_element(18, "微信公众号：代码随想录"); 
	skipList.insert_element(9, "学习"); 
	skipList.insert_element(49, "算法不迷路"); 
	skipList.insert_element(19, "赶快关注吧你会发现详见很晚！"); 

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