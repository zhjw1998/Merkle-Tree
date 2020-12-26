#include <iostream>
#include <iomanip>
#include "../include/merkle_tree.h"
#include <queue>
#include <vector>
#include <time.h>
#include "../include/node.h"
#define GETINDEX(c) ((c > '9') ? (c - 'a' + 10) : (c - '0'))

MerkleTree::MerkleTree(const std::vector<std::string>& leaves) {
    std::vector<Node*> blocks;
    for(auto leaf : leaves){
        blocks.push_back(new Node(leaf));
        blocks.back()->left = nullptr;
        blocks.back()->right = nullptr;
    }
    clock_t start, finish;
    double duration1, duration2;
    start = clock();
    
    createMerkleTree(blocks); // this->root
    finish = clock();
    duration1 = (double)(finish - start) / CLOCKS_PER_SEC;
    std::cout << std::setw(24) << duration1;
    // printMerkleTree();
    // std::cout << "tree created" << std::endl;

    start = clock();
    BuildTrieTree(leaves); // this->trieroot
    finish = clock();
    duration2 = (double)(finish - start) / CLOCKS_PER_SEC;
    std::cout << std::setw(24) << duration2;
    std::cout << std::setw(24) << duration2 / duration1;
    // LevelOrderTraverse();
    // std::cout << "trie tree created" << std::endl;
}

MerkleTree::~MerkleTree() {
    deleteTree(this->root, true);
    MakeEmpty(this->trieroot);
    // std::cout << "Tree deleted" << std::endl;
}

void MerkleTree::printMerkleTree(){
    this->printTree(this->root, 0);
}

void MerkleTree::printTree(Node* node, int index) {
    if (node) {
        if (node->left) {
            printTree(node->left, index + 4);
        }
        if (index) {
            std::cout << std::setw(index);
        }
        std::cout << node->hash[0] << "\n";
        if (node->right) {
             printTree(node->right, index + 4);
        }
    }
}

void MerkleTree::deleteTree(Node* node, bool deleaf) {
    if (node) {
        if(node->left) deleteTree(node->left, deleaf);
        if(node->right) deleteTree(node->right, deleaf);
        if(deleaf || node->left || node->right) {
            delete node;
            node = nullptr;    
        }
    }
}

bool MerkleTree::containsNode(const std::string& data){
    std::vector<Node*> leaves = getLeaves();
    for(auto leaf : leaves){
        if(leaf->hash == data) return true;
    }
    return false;
}

Node* MerkleTree::getErrorNode(Node* root2){
    Node* node1 = this->root;
    Node* node2 = root2;
    while(node1->hash != node2->hash){
        if(!(node2->left) || !(node2->right)) return node2;
        if(node1->left->hash != node2->left->hash){
            node1 = node1->left;
            node2 = node2->left;
        }
        else if(node1->right->hash != node2->right->hash){
            node1 = node1->right;
            node2 = node2->right;
        }
    }
    return nullptr;
}

std::vector<Node*> MerkleTree::getLeaves(){
    std::deque<Node*> dq;
    Node* front = this->root;
    dq.push_back(front);
    while(!dq.empty()){
        Node* front = dq.front();
        if(!(front->left) && !(front->right)) break;
        dq.pop_front();
        if(front->left) dq.push_back(front->left);
        if(front->right) dq.push_back(front->right);
    }
    return std::vector<Node*>(dq.begin(), dq.end());
}

void MerkleTree::createMerkleTree(std::vector<Node*> blocks){
    std::vector<Node*> nodes;
    while (blocks.size() != 1) {
        for (unsigned int l = 0, n = 0; l < blocks.size(); l = l + 2, n++) {
            if (l != blocks.size() - 1) { // checks for adjacent block
                nodes.push_back(new Node(hash_sha256(blocks[l]->hash + blocks[l+1]->hash))); // combine and hash adjacent blocks
                nodes[n]->left = blocks[l]; // assign children
                nodes[n]->right = blocks[l+1];
            } else {
                nodes.push_back(new Node(blocks[l]->hash));
                nodes[n]->left = blocks[l];
                nodes[n]->right = nullptr;
            }
        }
        blocks = nodes;
        nodes.clear();
    }
    // printNodeHashes(blocks);
    this->root = blocks[0];
}

void MerkleTree::insertNode(const std::string& data){
    std::vector<Node*> leaves = getLeaves();
    leaves.push_back(new Node(data));
    deleteTree(this->root, false);
    createMerkleTree(leaves);
    std::cout << "merkle insert OK" << std::endl;

    // trietree
    Insert(data);
    std::cout << "trie insert OK" << std::endl;
}

void MerkleTree::deleteNode(const std::string& data){
    std::vector<Node*> leaves = getLeaves();
    for(unsigned int i = 0; i < leaves.size(); i++){
        if(leaves[i]->hash == data){
            delete leaves[i];
            leaves.erase(leaves.begin() + i);
            break;
        }
    }
    deleteTree(this->root, false);
    createMerkleTree(leaves);

    // trietree
    // 有待商榷
    Remove(this->trieroot, data, 0, data.size());
}



// TrieTree
void MerkleTree::Insert(const std::string& word){
    TrieTreeNode *p = this->trieroot;
    for (unsigned int i = 0; i < word.size(); i++){
        // std::cout <<"i=" << i << " word[i]=" << word[i] <<" " << word << std::endl;
        if(p->child[GETINDEX(word[i])] == nullptr){
            p->child[GETINDEX(word[i])] = new TrieTreeNode(word[i]);
            p->childCnt++;
        }
        //notice!!this line need to be optimized to handle with duplicated insertion
        p->child[GETINDEX(word[i])]->prefixCnt++;
        p = p->child[GETINDEX(word[i])];
    }
    p->isEnd = true;
}

void MerkleTree::BuildTrieTree(const std::vector<std::string>& words){
    this->trieroot = new TrieTreeNode('\0');
    for (unsigned int i = 0; i < words.size(); i++){
        Insert(words[i]);
    }
}

bool MerkleTree::Find(const std::string& word){
    TrieTreeNode *p = this->trieroot;
    for (unsigned int i = 0; i < word.size(); i++){ 
        if (p->child[GETINDEX(word[i])] == nullptr)
            return false;
        p = p->child[GETINDEX(word[i])];
    }
    return p->isEnd;
}

void MerkleTree::LevelOrderTraverse(){
    if(this->trieroot == nullptr)
        return;
    std::queue<TrieTreeNode *> Q;
    Q.push(this->trieroot);
    while (!Q.empty()){
        TrieTreeNode *p = Q.front();
        std::cout << p->val << "(" << p->childCnt << ") ";
        for (int i = 0; i < SIZE; i++){
            if(p->child[i] != nullptr)
                Q.push(p->child[i]);
        }
        Q.pop();
    }
    std::cout << "\n";
}

void MerkleTree::PreOrderTraverse(){
    PreTraverse(this->trieroot);
    std::cout<<std::endl;
}

void MerkleTree::PostOrderTraverse(){
    PostTraverse(this->trieroot);
    std::cout<<std::endl;
}

bool MerkleTree::Remove(TrieTreeNode *treeNode, const std::string& word,int pos, int n) {
    if (pos == n){
        treeNode->isEnd = false;//set the node not to be an end
        //if the last node contains the last char is a leaf,return true to delete it
        return treeNode->childCnt == 0;
    }
    //not found, not delete this node
    if (treeNode->child[GETINDEX(word[pos])] == nullptr) 
        return false;
    //if true, the child is a leaf, delete the child
    if ( Remove( treeNode->child[GETINDEX(word[pos])], word, pos+1, n)){ 
        delete treeNode->child[GETINDEX(word[pos])];
        treeNode->child[GETINDEX(word[pos])] = nullptr;
 
        treeNode->prefixCnt--;
        //if the node becomes a leaf and is not an end,return true to delete it
        if (--treeNode->childCnt == 0 && treeNode->isEnd == false)
            return true;
    }
 
    //other not delete
    return false;
}
 
//Count the number of words which contain the specific prefix
int MerkleTree::CountWordsWithPrefix(const std::string& prefix){
    TrieTreeNode *p = this->trieroot;
    for (unsigned int i = 0; i < prefix.size(); i++){
        if(p->child[GETINDEX(prefix[i])] == nullptr)
            return 0;
        
        p = p->child[GETINDEX(prefix[i])];
    }
    return p->prefixCnt;
}

void MerkleTree::PreTraverse(TrieTreeNode *treeNode){
    if (treeNode != nullptr){
        std::cout << treeNode->val << "(" << treeNode->childCnt << ") ";
        for (int i = 0; i < SIZE; i++){
            PreTraverse(treeNode->child[i]);
        }
    }
}
 
void MerkleTree::PostTraverse(TrieTreeNode *treeNode){
    if (treeNode != nullptr){
        for (int i = 0; i < SIZE; i++){
            PostTraverse(treeNode->child[i]);
        }
        std::cout << treeNode->val << "(" << treeNode->childCnt << ") ";
    }
}
 
void MerkleTree::MakeEmpty(TrieTreeNode *treeNode){
    if (treeNode != nullptr){
        for (int i = 0; i < SIZE; i++){
            MakeEmpty(treeNode->child[i]);
        }
        delete treeNode;
    }
    treeNode = nullptr;
}

// int test(){
//     TrieTreeNode *root = new TrieTreeNode('\0');
//     //const char *words[] = {"b","abc","abd","bcd","abcd","efg","hii"}; //test insert
//     //cout << sizeof(words) << "\n";//(4*7=28)
//     //cout << sizeof(words[0]) << "\n";//4(is a pointer)
 
//     const string words[] = {"abc","ad","ef"};//test remove
//     BuildTrieTree(root,words,sizeof(words)/sizeof(words[0]));
//     LevelOrderTraverse(root);
//     PreOrderTraverse(root);
//     cout << '\n';
//     PostOrderTraverse(root);
//     cout << "\n";
 
//     if (Find(root,"ef"))
//         cout << "ef found" << endl;
//     else cout << "ef not found" <<endl;
 
//     Insert(root,"e");
//     //after this insertion.the node 'e' becomes a end but it's not a leaf,
//     //so it can not be deleted unless its leaf(leaves) is deleted
//     LevelOrderTraverse(root);
 
//     Remove(root,"ef",0,strlen("ef"));
//     LevelOrderTraverse(root);
 
//     Remove(root,"e",0,strlen("e"));
//     LevelOrderTraverse(root);
 
//     cout << CountWordsWithPrefix(root,"a")<<endl;
//     Remove(root,"ad",0,strlen("ad"));
//     cout << CountWordsWithPrefix(root,"a")<<endl;
 
//     MakeEmpty(root);
    
//     return 0;
// }