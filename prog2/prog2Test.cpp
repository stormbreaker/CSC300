#include <iomanip>
#include <iostream>
#include <fstream>

using namespace std;

struct avl_tree_node
  {
	  int  key_value;
	  int  left_child;    // child file address
                      // [record index, not byte address]
	  int  right_child; 
	  int  height;        // height from here down (0 if leaf)
	  int  parent;        // record index of parent (C_NULL if root)
	  // may need filler to force zeros in any padding of the struct
	  int  file_loc;      // location on file (record index)
   };
const int C_NULL = -1;
const int C_NODE_SIZE = sizeof(avl_tree_node);

bool insert(avl_tree_node &node, fstream &file, int nodeloc, bool &flag, avl_tree_node &root);
//bool checkbal(); // needs parameters and is recursive
//bool rotate(); // needs paramters and swaps pointer vals


int main(int argc, char** argv)
{
	ifstream fin;
	fstream finout;

	avl_tree_node tempnode; //This is our max number of nodes held in memory
	avl_tree_node root;
	root.key_value = C_NULL;
	root.left_child = C_NULL;
	root.right_child = C_NULL;
	root.height = C_NULL;
	root.parent = C_NULL;
	root.file_loc = C_NULL;
	int count = 0;
	bool flag = false;

	fin.open( "Sample.txt"/*argv[1]*/);
	finout.open("out.bin"/*argv[2]*/, ios::in | ios:: out | ios::app | ios::binary);
	//tempnode[0]  is always the one that operations are being performed on
	while (fin >> tempnode.key_value)
	{
		tempnode.file_loc = count;
		//insert
		insert(tempnode, finout, 0, flag, root);//passes key value set and all other fields junk except file_loc
		count++;
	}

	finout.close();
	fin.close();

	return 0;
}

bool insert(avl_tree_node &node, fstream &file, int nodeloc, bool &flag, avl_tree_node &root)//node is the node being written
{	
	if (flag == true || node.file_loc == 0)
	{
		node.left_child = C_NULL;
		node.right_child = C_NULL;
		node.height = 0;
		node.parent = root.file_loc;
		file.seekg(node.file_loc * C_NODE_SIZE);
		file.write((char*) &node.key_value, sizeof(int));
		file.write((char*) &node.left_child, sizeof(int));
		file.write((char*) &node.right_child, sizeof(int));
		file.write((char*) &node.height, sizeof(int));
		file.write((char*) &node.parent, sizeof(int));
		file.write((char*) &node.file_loc, sizeof(int));
		return true;
	}

	file.seekg(nodeloc * C_NODE_SIZE);

	//need case to handle empty file -- is this our basis case????
	file.read((char*) &root.key_value, sizeof(int));
	file.read((char*) &root.left_child, sizeof(int)); //read left_child (pointer)
	file.read((char*) &root.right_child, sizeof(int));//read right_child (pointer)
	file.read((char*) &root.height, sizeof(int));//read height (pointer)
	file.read((char*) &root.parent, sizeof(int));//read parent
	file.read((char*) &root.file_loc, sizeof(int));

	if (node.key_value < root.key_value)
	{
		if (root.left_child == C_NULL)
		{
				flag = true;
				node.left_child = C_NULL;
				node.right_child = C_NULL;
				node.height = 0;
				node.parent = root.file_loc;
				if (root.height = 0)
				{
					root.height += 1;
				}
				root.left_child = node.file_loc;
				file.seekg(root.file_loc * C_NODE_SIZE);
				
			file.write((char*) &root.key_value, sizeof(int));
			file.write((char*) &root.left_child, sizeof(int));
			file.write((char*) &root.right_child, sizeof(int));
			file.write((char*) &root.height, sizeof(int));
			file.write((char*) &root.parent, sizeof(int));
			file.write((char*) &root.file_loc, sizeof(int));
				insert(node, file, 0, flag, root);
			}
			else
			{
				// flag should stay false
				insert(node, file, root.left_child, flag, root);
			}
		}
		else
		{
			if (root.right_child == C_NULL)
			{
				flag = true;
				node.left_child = C_NULL;
				node.right_child = C_NULL;
				node.height = 0;
				node.parent = root.file_loc;
				if (root.height = 0)
				{
					root.height += 1;
				}
				root.right_child = node.file_loc;
				file.seekg(root.file_loc * C_NODE_SIZE);
			file.write((char*) &root.key_value, sizeof(int));
			file.write((char*) &root.left_child, sizeof(int));
			file.write((char*) &root.right_child, sizeof(int));
			file.write((char*) &root.height, sizeof(int));
			file.write((char*) &root.parent, sizeof(int));
			file.write((char*) &root.file_loc, sizeof(int));
				insert(node, file, 0, flag, root);
			}
			else
			{
				//flag should stay false
				insert(node, file, root.right_child, flag, root);
			}
		}
}
//bool checkbal(//pass array?) // needs paramters and is recursive
//{
 //this compares heights of subtrees and also calls rotate
//}
//bool rotate(// pass array?) // needs parameters and swaps pointer vals
//{
 //will be called by checkbal
//}
