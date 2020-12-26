#include "../include/node.h"
#include <cstring>

    Node::Node(std::string data){
    	this->hash = data;
    	this->left = this->right = nullptr;
    }
    TrieTreeNode::TrieTreeNode(char _val):val(_val),isEnd(false),childCnt(0),prefixCnt(0){
		memset(child, 0, sizeof(child));
		for(int i = 0; i < SIZE; i++)
			child[i] = nullptr;
	}