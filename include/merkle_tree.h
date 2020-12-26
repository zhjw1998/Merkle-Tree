#ifndef MERKLE_TREE_H
#define MERKLE_TREE_H

#include "node.h"
#include "picosha2.h"
#include "misc.h"
#include <vector>
#include <string>

class MerkleTree {
public:
	// MerkleTree
    Node* root;
    MerkleTree(const std::vector<std::string>& blocks);
    ~MerkleTree();
    void printMerkleTree();
    void deleteTree(Node *n, bool deleaf);
    // 判断某一节点是否在merkele-tree中，传递该数据的hash值
    bool containsNode(const std::string& data); 
    Node* getErrorNode(Node* root2); // 找出root1和root2中不相同的节点   这个感觉没啥用
    void insertNode(const std::string& data);//插入节点
    void deleteNode(const std::string& data);//删除节点

    // TrieTree
    TrieTreeNode *trieroot;
    void Insert(const std::string& word);
    bool Remove(TrieTreeNode *treeNode, const std::string& word,int pos, int n);
	bool Find(const std::string& word);
	void LevelOrderTraverse();
	void BuildTrieTree(const std::vector<std::string>& words);
	int CountWordsWithPrefix(const std::string& prefix);
	void PreOrderTraverse();
	void PostOrderTraverse();
	
	//Remove还没看懂
	
private:
	void printTree(Node *n, int indent);
	std::vector<Node*> getLeaves(); // 获取叶节点的列表
	void createMerkleTree(std::vector<Node*> blocks);// 创建Merkle树

	void PreTraverse(TrieTreeNode *treeNode);
	void PostTraverse(TrieTreeNode *treeNode);
	void MakeEmpty(TrieTreeNode *treeNode);
};
#endif /* MERKLE_TREE_H */
