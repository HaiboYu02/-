#include <iostream>
#include "tree.h"
#include "sha256.h"
using namespace std;

int main()
{
	string check_str = "";
	int node_num;
	cout << "输入 Merkle Hash Tree 的叶子结点的数量：" << endl;
	cin >> node_num;
	cout << "输入 Merkle Hash Tree 的叶子结点的数据: " << endl;
	vector<string> v;

	while (node_num > 0) //输入一个序列作为叶子节点的值，可以为乱序
	{
		string str;
		cin >> str;
		v.push_back(str);
		node_num--;
	}

	tree ntree;
	sort(v.begin(), v.end());
	ntree.buildBaseLeafes(v);
	cout << "构建 Merkle Hash Tree :" << endl << endl;
	ntree.buildTree();

	//Merkle Tree–Inclusion Proof
	string strr = "*";
	cout << strr;
	cout << endl;
	cout << "想验证的数据: " << endl;
	cin >> check_str;   //输入想验证的叶子节点
	check_str = sha2::hash256_hex_string("00" + check_str);

	if (ntree.verify(check_str))   //验证有无这个节点，并打印该节点到树根节点的Merkle Audit Path
	{
		cout << endl << endl;
		cout << "Merkle Tree上存在想验证的数据的叶子结点" << endl;
	}
	else
	{
		cout << "Merkle Tree上不存在想验证的数据的叶子结点" << endl;
	}
	return 0;
}
