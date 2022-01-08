#ifndef _H_PRINT_TREE_H_
#define _H_PRINT_TREE_H_

#include <iostream>
#include <string>

using std::cout;
using std::string;
using std::endl;

static string str_tree;

template <class NodePtr>
class CPrintBTree
{
	static string data_str(NodePtr node)
	{
		 return 
			 std::to_string(node->value.first)
			 + "|" 
			 + (std::to_string(node->value.second)
			 + "|" 
			 + ((bool)node->color ? "B" : "R"));
	}

	static void horizontalStyle(NodePtr root, bool isRight, string indent)
    {
		if (root->node_right() != 0) {
			horizontalStyle(root->node_right(), true, indent + (isRight ? "        " : " |      "));
		}
		str_tree += indent;
		str_tree += isRight ? " /" : " \\";
		str_tree += "----- ";
		str_tree += data_str(root);
		str_tree += "\n";
		
		if (root->node_left() != 0) {
			horizontalStyle(root->node_left(), false, indent + (isRight ? " |      " : "        "));
		}
    }

public:
	static void print(NodePtr root)
    {
		str_tree.clear();

		if (!root)
			return;

		if (root->node_right() != 0) {
			horizontalStyle(root->node_right(), true, "");
		}

        str_tree += data_str(root);
		str_tree += '\n';

		if (root->node_left() != 0) {
			horizontalStyle(root->node_left(), false, "");
		}

        cout << str_tree << endl;
    }

};

#endif // _H_PRINT_TREE_H_