#ifndef _H_PRINT_TREE_H_
#define _H_PRINT_TREE_H_

#include <iostream>
#include <string>

using std::cout;
using std::string;
using std::endl;

static string strTree;

template <class NodePtr>
class CPrintBTree
{
	static void horizontalStyle(NodePtr root, bool isRight, string indent)
    {
		if (root->pRight != 0) {
			horizontalStyle(root->Right(), true, indent + (isRight ? "        " : " |      "));
		}
		strTree += indent;
		strTree += isRight ? " /" : " \\";
		strTree += "----- ";
		strTree += (std::to_string(root->value.first) + "|" + ((bool)root->bColor ? "B" : "R"));
		strTree += "\n";
		
		if (root->pLeft != 0) {
			horizontalStyle(root->Left(), false, indent + (isRight ? " |      " : "        "));
		}
    }

public:
	static void Print(NodePtr root)
    {
		strTree.clear();

		if (!root)
			return;

		if (root->pRight != 0) {
			horizontalStyle(root->Right(), true, "");
		}

        strTree += (std::to_string(root->value.first) + "|" + ((bool)root->bColor ? "B" : "R") + "\n");

		if (root->pLeft != 0) {
			horizontalStyle(root->Left(), false, "");
		}

        cout << strTree << endl;
    }

};

#endif // _H_PRINT_TREE_H_