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

bool insert(avl_tree_node &node, fstream &file, int nodeloc, bool &flag, avl_tree_node &root, bool &height, avl_tree_node &temp, avl_tree_node &left,
avl_tree_node &right);
void bal(avl_tree_node &root, fstream &file, avl_tree_node &left, avl_tree_node &right, avl_tree_node &node); // needs parameters
//bool rotate(); // needs paramters and swaps pointer vals


int main(int argc, char** argv)
{
	ifstream fin;
	fstream finout;
	cout << "yeah i don't know";
	avl_tree_node tempnode; //This is our max number of nodes held in memory
	avl_tree_node root;
	avl_tree_node temp, left, right;
	root.key_value = C_NULL;
	root.left_child = C_NULL;
	root.right_child = C_NULL;
	root.height = C_NULL;
	root.parent = C_NULL;
	root.file_loc = C_NULL;
	int count = 0;
	bool flag = false;
	bool height = false;
	cout << "hahaha";
	fin.open( "Sample.txt"/*argv[1]*/);
	finout.open("out.bin"/*argv[2]*/, ios::in | ios:: out | ios::ate | ios::trunc);
	//tempnode[0]  is always the one that operations are being performed on
	while (fin >> tempnode.key_value)
	{
		cout << "enter loop";
		tempnode.file_loc = count;
		//insert
		flag = false;
		height = false;
		insert(tempnode, finout, 0, flag, root, height, temp, left, right);//passes key value set and all other fields junk except file_loc
		count++;
		cout << count;
	}

	finout.close();
	fin.close();

	return 0;
}

bool insert(avl_tree_node &node, fstream &file, int nodeloc, bool &flag, avl_tree_node &root, bool &height, avl_tree_node &temp, avl_tree_node &left,
avl_tree_node &right)//node is the node being written
{
		
	if (flag == true || node.file_loc == 0)
	{
		node.left_child = C_NULL;
		node.right_child = C_NULL;
		node.height = 0;
		node.parent = root.file_loc;
		file.seekg(node.file_loc * C_NODE_SIZE);
		file.write((char*) &node, sizeof(avl_tree_node));
		file.seekg(node.file_loc * C_NODE_SIZE);
		file.read((char*) &root, sizeof(avl_tree_node));
		return flag;
	}

	file.seekg(nodeloc * C_NODE_SIZE);

	//need case to handle empty file -- is this our basis case????
	file.read((char*) &root, sizeof(avl_tree_node));

	if (node.key_value < root.key_value)
	{
		if (root.left_child == C_NULL)
		{
			flag = true;
			node.left_child = C_NULL;
			node.right_child = C_NULL;
			node.height = 0;
			node.parent = root.file_loc;
			root.left_child = node.file_loc;
			file.seekg(root.file_loc * C_NODE_SIZE);
			file.write((char*) &root, sizeof(avl_tree_node));	
			insert(node, file, 0, flag, root, height, temp, left, right);
		}
		else
		{
			// flag should stay false
			insert(node, file, root.left_child, flag, root, height, temp, left, right);
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
			root.right_child = node.file_loc;
			file.seekg(root.file_loc * C_NODE_SIZE);
			file.write((char*) &root, sizeof(avl_tree_node));
			insert(node, file, 0, flag, root, height, temp, left, right);
		}
		else
		{
			//flag should stay false
			insert(node, file, root.right_child, flag, root, height, temp, left, right);
		}
	}
	cout << "hi";
	if (root.parent != C_NULL)
	{
		/*file.seekg(root.file_loc * C_NODE_SIZE);
		file.read((char*) &root, sizeof(avl_tree_node));
		*/ //what the heck is this?
		file.seekg(root.parent * C_NODE_SIZE);
		file.read((char*) &temp, sizeof(avl_tree_node));
		temp.height = root.height + 1;
		file.seekg(temp.file_loc * C_NODE_SIZE);
		file.write((char*) &temp, sizeof(avl_tree_node));
		file.seekg(temp.file_loc * C_NODE_SIZE);
		file.read((char*) &root, sizeof(avl_tree_node));
	}
	cout << "justb4call";
	if(root.file_loc != 0)
	{
		cout << "called";
		bal(root, file, left, right, node);
	}
	else
	{
		//handle root
	}
	return flag;		
}
void bal(avl_tree_node &root, fstream &file, avl_tree_node &left, avl_tree_node &right, avl_tree_node &node) // needs paramters
{
	if (root.left_child == C_NULL)
	{
		left.height = C_NULL;
	}
	else
	{
		file.seekg(root.left_child * C_NODE_SIZE);
		file.read((char*) &left, sizeof(avl_tree_node));
	}
	if (root.right_child == C_NULL)
	{
		right.height = C_NULL;
	}
	else
	{
		file.seekg(root.right_child * C_NODE_SIZE);
		file.read((char*) &right, sizeof(avl_tree_node));
	}
	if (left.height - right.height > 1 || left.height - right.height < C_NULL)
	{
	/*	if (right.height == C_NULL && left.left_child != C_NULL) // left left
		{	
			cout << "leftleft";
			file.seekg(left.left_child * C_NODE_SIZE);
			file.read((char*) &right, sizeof(avl_tree_node));
			left.parent = root.parent;
			left.right_child = root.file_loc;
			root.parent = left.file_loc;
			root.height = 0;
			file.seekg(left.parent * C_NODE_SIZE);
		       	file.read((char*) &node, sizeof(avl_tree_node));
			if(node.left_child == root.file_loc)
			  {
			    node.left_child = left.file_loc;
			  }
			else
			  {
			    node.right_child = left.file_loc;
			  }
			file.seekg(node.file_loc * C_NODE_SIZE);
		       	file.write((char*) &node, sizeof(avl_tree_node));
		}
		else if (right.height == C_NULL && left.right_child != C_NULL)//left right
		{	
			cout << "leftright";
			file.seekg(left.right_child * C_NODE_SIZE);
			file.read((char*) & right, sizeof(avl_tree_node));
			right.parent = root.parent;
			right.right_child = root.file_loc;
			right.left_child = left.file_loc;
			root.height = 0;
			right.height = 1;
			left.height = 0;
			file.seekg(left.parent * C_NODE_SIZE);
		       	file.read((char*) &node, sizeof(avl_tree_node));
			if(node.left_child == root.file_loc)
			  {
			    node.left_child = right.file_loc;
			  }
			else
			  {
			    node.right_child = right.file_loc;
			  }
			file.seekg(node.file_loc * C_NODE_SIZE);
		       	file.write((char*) &node, sizeof(avl_tree_node));	
		}
		else if (left.height == C_NULL && right.right_child != C_NULL)// right right
		{
			cout << "rightright";
			file.seekg(right.right_child * C_NODE_SIZE);
			file.read((char*) &left, sizeof(avl_tree_node));
			right.parent = root.parent;
			right.left_child = root.file_loc;
			root.height = 0;
			file.seekg(left.parent * C_NODE_SIZE);
		       	file.read((char*) &node, sizeof(avl_tree_node));
			if(node.left_child == root.file_loc)
			  {
			    node.left_child = right.file_loc;
			  }
			else
			  {
			    node.right_child = right.file_loc;
			  }
			file.seekg(node.file_loc * C_NODE_SIZE);
		       	file.write((char*) &node, sizeof(avl_tree_node));
		}
		else if (left.height == C_NULL & right.left_child != C_NULL)// right left
		{
			cout << "rightleft";
			file.seekg(right.left_child * C_NODE_SIZE);
			file.read((char*) &left, sizeof(avl_tree_node));
			left.parent = root.parent;
			left.left_child = root.file_loc;
			left.right_child = right.file_loc;
			root.height = 0;
			right.height = 0;
			left.height = 1;
			file.seekg(left.parent * C_NODE_SIZE);
		       	file.read((char*) &node, sizeof(avl_tree_node));
			if(node.left_child == root.file_loc)
			  {
			    node.left_child = left.file_loc;
			  }
			else
			  {
			    node.right_child = left.file_loc;
			  }
			file.seekg(node.file_loc * C_NODE_SIZE);
		       	file.write((char*) &node, sizeof(avl_tree_node));	
		}
	}
*//*
       	file.seekg(right.file_loc * C_NODE_SIZE);
      	file.write((char*) &right, sizeof(avl_tree_node));
       	file.seekg(left.file_loc * C_NODE_SIZE);
       	file.write((char*) &left, sizeof(avl_tree_node));
       	file.seekg(root.file_loc * C_NODE_SIZE);
       	file.write((char*) &root, sizeof(avl_tree_node));
*/
	}
}
