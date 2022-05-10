
#include<iostream>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<mutex>
#include<fstream>

#define STORE_FILE "store/dumpFile"

using namespace std;
mutex mtx;
string delimiter = ":";

//跳跃表节点模板
template<typename K, typename V>
class Node
{
private:
    K key;
    V value;
public:
    Node() {};

    Node(K k, V v, int);

    ~Node();

    K get_key() const;

    V get_value() const;

    void set_vaule(V);

    Node<K, V>** forward;

    int node_level;
    
};

template<typename K, typename V>
Node<K, V>::Node(const K k, const V v, int level) {
    this->key = k;
    this->value = v;
    this->node_level = level;

    this->forward = new Node<K, V>*[level + 1];

    memset(this->forward, 0, sizeof(Node<K, V>*)*(level + 1));

};

template<typename K, typename V>
Node<K, V>::~Node() {
    delete []forward;
};

template<typename K, typename V>
K Node<K, V>::get_key() const {
    return key;
};

template<typename K, typename V>
V Node<K, V>::get_value() const {
    return value;
};

template<typename K, typename V>
void Node<K, V>::set_vaule(V value) {
    this->value = value;
};

//SkipList 的模板类
template<typename K, typename V>
class SkipList {

public:
    SkipList(int);
    ~SkipList();
    int get_random_level();
    Node<K, V>* create_node(K, V, int);
    int insert_element(K, V);
    void display_list();
    bool search_element(K);
    void delete_element(K);
    void dump_file();
    void load_file();
    int get_size();

private:
    void get_key_vaule_from_string(const string& str, string* key, string* value);
    bool is_valid_string(const string& str);

private:
    //SkipList最大的层数
    int _max_level;

    //SkipList当前的层数
    int _skip_list_level;

    //表头节点指针
    Node<K, V>* _header;

    //文件操作符
    ofstream _file_writer;  //从内存到硬盘
    ifstream _file_reader;  //从硬盘到内存

    //SkipList当前元素数量
    int _element_count;

};


// 构造 SkipList
template<typename K, typename V> 
SkipList<K, V>::SkipList(int max_level) {
    this->_max_level = max_level;
    this->_skip_list_level = 0;
    this->_element_count = 0;

    //初始化K、V为0。创建前节点
    K k;
    V v;
    this->_header = new Node<K, V>(k, v, _max_level);
};


template<typename K, typename V> 
SkipList<K, V>::~SkipList() {
    if (_file_writer.is_open()) {
        _file_writer.close();
    }
    if (_file_reader.is_open()) {
        _file_reader.close();
    }
    delete _header;
}

template<typename K, typename V> 
int SkipList<K, V>::get_random_level() {
    int k = 1;
    while (rand() % 2) {
        ++k;
    }
    k = (k < _max_level) ? k : _max_level;
    return k;
}

// 得到跳跃表当前大小
template<typename K, typename V> 
int SkipList<K, V >::get_size() {
    return _element_count;
}

template<typename K, typename V> 
void SkipList<K, V>::get_key_vaule_from_string(const string& str, string* key, string* value) {
    if (!is_valid_string(str)) {
        return;
    }
    *key = str.substr(0, str.find(delimiter));
    *value = str.substr(str.find(delimiter) + 1, str.length());
}

template<typename K, typename V> 
bool SkipList<K, V>::is_valid_string(const string& str) {
    if (str.empty()) {
        return false;
    }
    if (str.find(delimiter) == string::npos) {
        return false;
    }
    return true;
}

//打印SkipList
template<typename K, typename V> 
void SkipList<K, V>::display_list() {
    cout << "\n       SkipList        \n";
    for (int i = 0; i <= _skip_list_level; ++i) {
        Node<K, V>* node = this->_header->forward[i];
        cout << " Level " << i << ": ";
        while (node != nullptr) {
            cout << node->get_key() << ":" << node->get_value() << ";";
            node = node->forward[i];
        }
        cout << endl;
    }
}

// 将数据转存到文件
template<typename K, typename V> 
void SkipList<K, V>::dump_file() {
    cout << "dump_file------------" << endl;
    _file_writer.open(STORE_FILE);
    Node<K, V>* node = this->_header->forward[0];

    while (node != nullptr) {
        _file_writer << node->get_key() << ":" << node->get_value() << "\n";
        cout << node->get_key() << ":" << node->get_value() << "\n";
        node = node->forward[0];
    }
    _file_writer.flush();
    _file_writer.close();
    return;

}

// 从磁盘中加载数据
template<typename K, typename V> 
void SkipList<K, V>::load_file() {
    _file_reader.open(STORE_FILE);
    cout << "load_file--------------" << endl;
    string line;
    string* key = new string();
    string* value = new string();
    while (getline(_file_reader, line)) {
        get_key_vaule_from_string(line, key, value);
        if (key->empty() || value->empty()) {
            continue;
        }
    }
}

template<typename K, typename V>
Node<K, V>* SkipList<K, V>::create_node(const K k, const V v, int level) {
    Node<K, V>* node = new Node<K, V>(k, v, level);
    return node;
};

template<typename K, typename V> 
int SkipList<K, V>::insert_element(const K key, const V value) {
    mtx.lock();
    Node<K, V>* current = this->_header;

    //创建更新数组
    Node<K, V>* update[_max_level + 1];
    memset(update, 0, sizeof(Node<K, V>*)*(_max_level + 1));

    //从跳跃表最高层开始
    for (int i = _skip_list_level; i >= 0; --i) {
        while (current->forward[i] != nullptr && current->forward[i]->get_key() < key) {
            current = current->forward[i];
        }
        update[i] = current;
    }

    current = current->forward[0];

    if (current != nullptr && current->get_key() == key) {
        cout << "key: " << key << ", exists" << endl;
        mtx.unlock();
        return 1;
    }

    if (current == nullptr || current->get_key() != key) {
        //随机产生个层数
        int random_level = get_random_level();

        if (random_level > _skip_list_level) {
            for (int i = _skip_list_level + 1; i < random_level + 1; ++i) {
                update[i] = _header;
            }
            _skip_list_level = random_level;
        }

        Node<K, V>* insert_node = create_node(key, value, random_level);

        //插入节点
        for (int i = 0; i < random_level + 1; ++i) {
            insert_node->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = insert_node;
        }
        cout << "Successfully inserted key:" << key << ", value:" << value << endl;
        _element_count ++;

    }
    mtx.unlock();
    return 0;

}

template<typename K, typename V> 
void SkipList<K, V>::delete_element(K key) {
    mtx.lock();
    Node<K, V>* current = this->_header;
    Node<K, V>* update[_max_level + 1];
    memset(update, 0, sizeof(Node<K, V>*)*(_max_level + 1));

    //
    for (int i = _skip_list_level; i >= 0; --i) {
        while (current->forward[i] != nullptr && current->forward[i]->get_key() < key) {
            current = current->forward[i];
        }
        update[i] = current;
    }

    current = current->forward[0];
    if (current != nullptr && current->get_key() == key) {
        //
        for (int i = 0; i < _skip_list_level + 1; ++i) {
            //
            if (update[i]->forward[i] != current) {
                break;
            }
            update[i]->forward[i] = current->forward[i];
        }
        //
        while (_skip_list_level > 0 && _header->forward[_skip_list_level] == 0) {
            --_skip_list_level;
        }

        cout << "Successfully deleted key "<< key << endl;
        --_element_count;

    }
    mtx.unlock();
    return;

}

template<typename K, typename V> 
bool SkipList<K, V>::search_element(K key) {
    cout << "search_element-----------" << endl;
    Node<K, V>* current = _header;

    //
    for (int i = _skip_list_level; i >= 0; --i) {
        while (current->forward[i] && current->forward[i]->get_key() < key) {
            current = current->forward[i];
        }
    }
    //
    current = current->forward[0];

    if (current && current->get_key() == key) {
        cout << "Found key: " << key << ", value: " << current->get_value() << endl;
        return false;
    }

    cout << "Not Found Key:" << key << endl;
    return false;
}



