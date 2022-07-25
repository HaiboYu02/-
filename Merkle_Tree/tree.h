#pragma once
#include "node.h"
#include <iostream>
#include "sha256.h"
using namespace std;
class tree
{
private:
	string merkleRoot;
	int makeBinary(vector<node*>& node_vector);
	void printTreeLevel(vector<node*> v);
	vector<vector<node*>> base;     //节点列表
public:
	tree();
	void buildTree();
	void buildBaseLeafes(vector<string> base_leafs);
	int verify(string hash);
	virtual ~tree();
};

tree::tree() {}

int tree::makeBinary(vector<node*>& node_vector) //使叶子节点数为偶数
{
	int vectSize = node_vector.size();
	if ((vectSize % 2) != 0) //如果元素个数为奇数，就把再最后一个节点再压入向量一次
	{
		node_vector.push_back(node_vector.end()[-1]);
		vectSize++;
	}
	return vectSize;
}

void tree::printTreeLevel(vector<node*> v)
{
	for (node* el : v)
	{
		cout << el->getHash() << endl;
	}
	cout << endl;
}

//构造merkle tree
void tree::buildTree() 
{
	do
	{
		vector<node*> new_nodes;
		makeBinary(base.end()[-1]); 

		for (int i = 0; i < base.end()[-1].size(); i += 2)
		{
			node* new_parent = new node;   //新建父亲节点 

			//设两个孩子节点的哈希值为h1、h2，则设置父亲节点哈希值为hash(0x01||h1||h2)
			new_parent->setHash("01" + base.end()[-1][i]->getHash() + base.end()[-1][i + 1]->getHash());
			
			//连接父亲节点和孩子节点
			base.end()[-1][i]->setParent(new_parent);
			base.end()[-1][i + 1]->setParent(new_parent);
			new_parent->setChildren(base.end()[-1][i], base.end()[-1][i + 1]);
			//将new_parent压入new_nodes
			new_nodes.push_back(new_parent);

			cout << "将 " << base.end()[-1][i]->getHash() << " 和 " << base.end()[-1][i + 1]->getHash() << " 连接,得到对应父节点的哈希值 " << endl;
		}

		cout << endl;
		cout << "得到的对应父节点的哈希值:" << endl;
		printTreeLevel(new_nodes);

		base.push_back(new_nodes); //将新一轮的父节点new_nodes压入base

		cout << "该层的结点有 " << base.end()[-1].size() << " 个:" << endl;
	} while (base.end()[-1].size() > 1); //这样每一轮得到新一层的父节点，知道得到根节点 退出循环

	merkleRoot = base.end()[-1][0]->getHash(); //根节点的哈希值

	cout << "Merkle Root : " << merkleRoot << endl << endl;
}

void tree::buildBaseLeafes(vector<string> base_leafs) //建立叶子节点列表
{
	vector<node*> new_nodes;

	cout << "叶子结点及对应的哈希值: " << endl;

	for (auto leaf : base_leafs) //给每一个字符串创建对应节点，并通过这个字符串设置哈希值
	{
		node* new_node = new node;
		//假设一个叶子节点的输入为d(n),则设置叶子节点哈希值为hash(0x00||d(n))
		new_node->setHash("00" + leaf);
		cout << leaf << ":" << new_node->getHash() << endl;

		new_nodes.push_back(new_node);
	}

	base.push_back(new_nodes);
	cout << endl;
}

int tree::verify(string hash)
{
	node* el_node = nullptr;
	string act_hash = hash;

	for (int i = 0; i < base[0].size(); i++)
	{
		if (base[0][i]->getHash() == hash)
		{
			el_node = base[0][i];
			break;
		}
	}
	if (el_node == nullptr)
	{
		return 0;
	}

	cout << "根据该节点的Merkle Audit Path计算根节点的值，使用到的节点哈希值:" << endl;
	cout << act_hash << endl;

	do  //验证merkle tree是否改变过：按照 Merkle Audit Path 重新计算根结点的值进行判断
	{
		if (el_node->checkDir() == 0)
		{
			act_hash = sha2::hash256_hex_string("01" + act_hash + el_node->getSibling()->getHash());
		}
		else
		{
			act_hash = sha2::hash256_hex_string("01" + el_node->getSibling()->getHash() + act_hash);
		}

		std::cout << act_hash << endl;

		el_node = el_node->getParent();
	} while ((el_node->getParent()) != NULL); //到达根节点

	return act_hash == merkleRoot ? 1 : 0;   
}

tree::~tree() {}
