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
void rootbal(avl_tree_node &root, fstream, &file, avl_tree_node &left, avl_tree_node &right);


int main(int argc, char** argv)
{
	ifstream fin;
	fstream finout;
	
	avl_tree_node tempnode; //node to be inserted
	avl_tree_node root;
	avl_tree_node temp, left, right;
	
	root.key_value = C_NULL;
	root.left_child = C_NULL;
	root.right_child = C_NULL;
	root.height = C_NULL;
	root.parent = C_NULL;
	root.file_loc = C_NULL;
	
	temp.key_value = C_NULL;
	temp.left_child = C_NULL;
	temp.right_child = C_NULL;
	temp.height = C_NULL;
	temp.parent = C_NULL;
	temp.file_loc = C_NULL;
	
	left.key_value = C_NULL;
	left.left_child = C_NULL;
	left.right_child = C_NULL;
	left.height = C_NULL;
	left.parent = C_NULL;
	left.file_loc = C_NULL;

	right.key_value = C_NULL;
	right.left_child = C_NULL;
	right.right_child = C_NULL;
	right.height = C_NULL;
	right.parent = C_NULL;
	right.file_loc = C_NULL;


	int count = 0;
	bool flag = false;
	bool height = false;
	
	fin.open( "Sample.txt"/*argv[1]*/);
	finout.open("out.bin"/*argv[2]*/, ios::in | ios:: out | ios::ate | ios::trunc);
	//tempnode[0]  is always the one that operations are being performed on
	while (fin >> tempnode.key_value)
	{
		cout << count << " ";
		tempnode.file_loc = count;
		flag = false;
		height = false;
		insert(tempnode, finout, 0, flag, root, height, temp, left, right);//passes key value set and all other fields junk except file_loc
		count++; // this is where i start for next evaluation
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
	file.read((char*) &root, sizeof(avl_tree_node));//root read twice? once on recursion exit but also here.  Only first time through????

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
			insert(node, file, 0, flag, root, height, temp, left, right);//entered recursion level iii here
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
			insert(node, file, root.right_child, flag, root, height, temp, left, right);//entered recursion level ii here
		}
	}
	if (root.parent != C_NULL)
	{
		
		file.seekg(root.parent * C_NODE_SIZE);
		file.read((char*) &temp, sizeof(avl_tree_node));
		temp.height = root.height + 1;
		file.seekg(temp.file_loc * C_NODE_SIZE);
		file.write((char*) &temp, sizeof(avl_tree_node));
		file.seekg(temp.file_loc * C_NODE_SIZE);
		file.read((char*) &root, sizeof(avl_tree_node));
	}
	cout << "justb4call" << endl;
	if(root.parent != C_NULL)
	{
		cout << "called" << endl;
		bal(root, file, left, right, node);//entered bal here
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
		if(left.height < right.height)//right cases
		{
			if (right.left_child != C_NULL)
			{
				file.seekg(right.left_child * C_NODE_SIZE);
				file.read((char*) &left, sizeof(avl_tree_node));
			}
			else
			{
				left.height = C_NULL;
			}
			if (right.right_child != C_NULL)
			{
				file.seekg(right.right_child * C_NODE_SIZE);
				file.read((char*) &node, sizeof(avl_tree_node));
			}
			else
			{
				node.height = C_NULL;
			}
			if(left.height <= node.height) //right right
			{
				right.parent = root.parent;
				root.right_child = right.left_child;
				if (right.left_child != C_NULL)
				{
					left.parent = root.file_loc;
				}
				root.parent = right.file_loc;
				right.left_child = root.file_loc;
				file.seekg(right.parent * C_NODE_SIZE);
				file.read((char*) &node, sizeof(avl_tree_node));
				node.right_child = right.file_loc;
				node.height = right.height + 1;
				if(root.left_child != C_NULL)
				{
					file.seekg(root.left_child * C_NODE_SIZE);
					file.read((char*) &node, sizeof(avl_tree_node));
					root.height = node.height + 1;
				}
				else
				{
					if(right.left_child != C_NULL)
					{
						node.height = left.height + 1;
					}
					else
					{
						node.height = 0;
					}
				}
			}//end right right	
		}//end right cases
		else
		{
			if(left.right_child != C_NULL)
			{
				file.seekg(left.right_child * C_NODE_SIZE);
				file.read((char*) &right, sizeof(avl_tree_node));
			}
			else
			{
				right.height = C_NULL;
			}
			if(left.left_child != C_NULL)
			{
				file.seekg(left.left_child * C_NODE_SIZE);
				file.read((char*) &node, sizeof(avl_tree_node));
			}
			else
			{
				node.height = C_NULL;
			}
			if (left.height >= node.height)// left left
			{
				left.parent = root.parent;
				root.left_child - left.right_child;
				if(left.right_child != C_NULL)
				{
					right.parent = root.file_loc;
				}
				root.parent = left.file_loc;
				file.seekg(left.parent * C_NODE_SIZE);
				file.read((char*) &node, sizeof(avl_tree_node));
				node.left_child = left.file_loc;
				node.height = left.height + 1;
				if(root.right_child != C_NULL)
				{
					file.seekg(root.right_child * C_NODE_SIZE);
					file.read((char*) &node, sizeof(avl_tree_node));
					root.height = node.height + 1;
				}
				else
				{
					if(left.right_child != C_NULL)
					{
						node.height = right.height + 1;
					}
					else
					{
						node.height = 0;
					}
				}
			}//end left left
		}//end left cases
	}
	if (right.file_loc != C_NULL)
	{
	       	file.seekg(right.file_loc * C_NODE_SIZE);
	      	file.write((char*) &right, sizeof(avl_tree_node));
	}
	if (left.file_loc != C_NULL)
	{       
		file.seekg(left.file_loc * C_NODE_SIZE);
	       	file.write((char*) &left, sizeof(avl_tree_node));
	}
       	file.seekg(root.file_loc * C_NODE_SIZE);
       	file.write((char*) &root, sizeof(avl_tree_node));
	right.key_value = C_NULL;
	right.left_child = C_NULL;
	right.right_child = C_NULL;
	right.height = C_NULL;
	right.parent = C_NULL;
	right.file_loc = C_NULL;
	left.key_value = C_NULL;
	left.left_child = C_NULL;
	left.right_child = C_NULL;
	left.height = C_NULL;
	left.parent = C_NULL;
	left.file_loc = C_NULL;	
}
void rootbal(avl_tree_node &root, fstream &file, avl_tree_node &left, avl_tree_node &right,avl_tree_node &node)
{
	int temp;
	if (root.right_child != C_NULL)
	{
		file.seekg(root.right_child * C_NODE_SIZE);
		file.read((char*) &right, sizeof(avl_tree_node));
	}
	else
	{
		right.height = C_NULL;
	}
	if (root.left_child != C_NULL)
	{
		file.seekg(root.left_child *C_NODE_SIZE);
		file.read((char*) &left, sizeof(avl_tree_node));
	}
	else
	{
		left.height = C_NULL;
	}
	if (left.height - right.height < C_NULL || left.height - right.height > 1)
	{
		if (left.height < right.height)
		{	
			if (right.left_child != C_NULL)
			{
				file.seekg(right.left_child * C_NODE_SIZE);
				file.read((char*) &left, sizeof(avl_tree_node));
			}
			else
			{
				left.height = C_NULL;
			}
			if (right.right_child != C_NULL)
			{
				file.seekg(right.right_child * C_NODE_SIZE);
				file.read((char*) &node, sizeof(avl_tree_node));
			}
			else
			{
				node.height = C_NULL;
			}
			if (left.height <= node.height)	//right right
			{
				//	root's right child becomes root
				temp = root.key_value;
				root.key_value = right.key_value;
				right.key_value = temp;
				root.right_child = right.right_child;
				node.parent = root.file_loc;
				root.height = node.height + 1;
				right.right_child = right.left_child
				right.left_child = root.left_child;
				if (right.left_child != C_NULL)
				{
					file.seekg(right.left_child * C_NODE_SIZE);
					file.read((char*) &node, sizeof(avl_tree));
					node.parent = right.file_loc;
					right.height = node.height + 1;
				}
				root.left_child = right.file_loc;
			}
			else if (left.height > node.height) // right left
			{
				// i'm confused
			}
		}	 	
		else if (left.height > right.height)
		{
			if(left.right_child != C_NULL)
			{
				file.seekg(left.right_child * C_NODE_SIZE);
				file.read((char*) &right, sizeof(avl_tree_node));
			}
			else
			{
				right.height = C_NULL;
			}
			if (left.left_child != C_NULL)
			{
				file.seekg(left.left_child * C_NODE_SIZE);
				file.read((char*) &node, sizeof(avl_tree_node));
			}
			else
			{
				node.height = C_NULL;
			}
			if (right.height <= node.height)//left left
			{
				temp = root.key_value;
				root.key_value = left.key_value;
				left.key_value = temp;
				root.left_child = left.left_child;
				node.parent = root.file_loc;
				root.height = node.height + 1;
				left.left_child = left.right_child;
				left.right_child = root.left_child;
				if(left.right_child != C_NULL)
				{
					file.seekg(left.right_child * C_NODE_SIZE);
					file.read((char*) &node, sizeof(avl_tree_node));
					node.parent = left.file_loc;
					left.height = node.height + 1;
				}
				root.right_child = left.file_loc;
			}
			else if (right.height > node.height) // left right
			{
				// I'm confused again
			}
		}
	}
}
