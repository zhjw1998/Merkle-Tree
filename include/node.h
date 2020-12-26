#ifndef NODE_H
#define NODE_H

#include <string>
struct Node {
    std::string hash;
    Node *left;
    Node *right;
    Node(std::string data);
};
const int SIZE = 16;
struct TrieTreeNode {
	char val;
	bool isEnd;
	int childCnt;
	int prefixCnt;
	TrieTreeNode *child[SIZE];
	TrieTreeNode(char _val);
};
#endif /* NODE_H */
