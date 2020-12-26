#include <iostream>
#include <iomanip>
#include "../include/merkle_tree.h"
#include <string>
#include <time.h>
void TimeCal(int nodenum){
    std::cout << std::setw(24) << nodenum;
    clock_t start, finish;
    double duration1, duration2;
    std::vector<std::string> leaves;
    for(int i = 0; i < nodenum; i++)
        leaves.push_back(hash_sha256(std::to_string(i)));

    MerkleTree* hashTree = new MerkleTree(leaves);

    start = clock();
    for(int i = 0; i < nodenum / 100; i++){
        hashTree->containsNode(hash_sha256(std::to_string(i * 100)));
    }
    finish = clock();
    duration1 = (double)(finish - start) / CLOCKS_PER_SEC;
    std::cout << std::setw(24) << (10000 * duration1) / nodenum;

    start = clock();
    for(int i = 0; i < nodenum / 100; i++){
        hashTree->Find(hash_sha256(std::to_string(i * 100)));
    }
    finish = clock();
    duration2 = (double)(finish - start) / CLOCKS_PER_SEC;
    std::cout << std::setw(24) << (10000 * duration2) / nodenum;

    std::cout << std::setw(24) << duration1 / duration2 << std::endl;
    //析构整个树
    delete hashTree;
}

void OptiTest(){
    std::cout << std::setw(24)  << "Node Num" << std::setw(24) << "Create Merkle Time" << std::setw(24) \
    << "Create Tire Time" << std::setw(24) << "SlowDown of Creation" << std::setw(24) <<"Search Merkel Tree" \
    << std::setw(24) << "Search Tire Tree" << std::setw(24) << "Speedup of Search" << std::endl;
    TimeCal(100);
    TimeCal(1000);
    TimeCal(10000);
    TimeCal(100000);
    TimeCal(1000000);
}

void InitLeaves(std::vector<std::string>& leaves){
    // 加入数据
    leaves.push_back(hash_sha256("abcdefg"));
    leaves.push_back(hash_sha256("qwerty12345"));
    leaves.push_back(hash_sha256("!@(@#)E&"));
    leaves.push_back(hash_sha256("ajosdhuhsdioa adsijd asjiod q9p0ad a."));
    leaves.push_back(hash_sha256("test01234"));
    leaves.push_back(hash_sha256("7846546521"));
    leaves.push_back(hash_sha256("asd970123+_?>?< f"));
    leaves.push_back(hash_sha256("op_) 4654asd21"));
    leaves.push_back(hash_sha256("12893 d970123+_qweawdf"));
}
void BasicTest(){
    // 构造
    std::vector<std::string> leaves;
    InitLeaves(leaves);
    std::cout << "-------Create tree-------" << std::endl;
    MerkleTree* hashTree = new MerkleTree(leaves);

    // 打印树
    // std::cout << "-------Print tree-------" << std::endl;
    // hashTree->printMerkleTree();

    // 查找节点
    std::cout << "-------Search node-------" << std::endl;
    std::vector<std::string> vs = {hash_sha256("op_) 4654asd21"), \
        hash_sha256("12893 d970123+_qweawdf"), hash_sha256("12893")};
    for(auto s : vs){
        std::cout<< s << ": ";
        if(hashTree->containsNode(s)) std::cout<< "true" << std::endl;
        else std::cout << "false" << std::endl;
    }

    hashTree->printMerkleTree();
    // 插入
    std::cout << "-------Insert node-------" << std::endl;
    std::string ins = hash_sha256("dkkdk384654asd21");
    std::cout << "insert hash: " << ins << std::endl;
    hashTree->insertNode(ins);
    hashTree->printMerkleTree();

    // 删除
    std::cout << "-------Delete node-------" << std::endl;
    std::string del = hash_sha256("!@(@#)E&");
    std::cout << "delete hash: " << del << std::endl;
    hashTree->deleteNode(del);
    hashTree->printMerkleTree();

    //析构整个树
    std::cout << "-------Delete tree-------" << std::endl;
    delete hashTree;
}

int main() {
    // 基本功能测试
    // BasicTest();
    // 优化比较
    OptiTest(); // 优化测试
    // BasicTest(); // 基本功能测试
}