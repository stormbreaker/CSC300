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
void rootbal(avl_tree_node &root, fstream &file, avl_tree_node &left, avl_tree_node &right,avl_tree_node &node);


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
	
	fin.open(argv[1]);
	finout.open(argv[2], ios::in | ios:: out | ios::ate | ios::trunc);
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
		if (node.file_loc != C_NULL)
		{
			file.seekg(node.file_loc * C_NODE_SIZE);
			file.write((char*) &node, sizeof(avl_tree_node));
		}
		if (node.file_loc != C_NULL)
		{
			file.seekg(node.file_loc * C_NODE_SIZE);
			file.read((char*) &root, sizeof(avl_tree_node));
		}
		cout << "succesful insert" << endl;	
		return flag;
	}

	if (nodeloc != C_NULL)
	{
		file.seekg(nodeloc * C_NODE_SIZE);
		file.read((char*) &root, sizeof(avl_tree_node));
	}

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
			if (root.file_loc != C_NULL);
			{		
				file.seekg(root.file_loc * C_NODE_SIZE);
				file.write((char*) &root, sizeof(avl_tree_node));
			}	
			insert(node, file, 0, flag, root, height, temp, left, right);
		}
		else
		{
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
			if (root.file_loc != C_NULL)
			{
				file.seekg(root.file_loc * C_NODE_SIZE);
				file.write((char*) &root, sizeof(avl_tree_node));
			}
			insert(node, file, 0, flag, root, height, temp, left, right);
		}
		else
		{
			insert(node, file, root.right_child, flag, root, height, temp, left, right);
		}
	}
	if (root.parent != C_NULL)
	{
		
		file.seekg(root.parent * C_NODE_SIZE);
		file.read((char*) &temp, sizeof(avl_tree_node));
		temp.height = root.height + 1;
		if (temp.file_loc != C_NULL)
		{
			file.seekg(temp.file_loc * C_NODE_SIZE);
			file.write((char*) &temp, sizeof(avl_tree_node));
		}
		if (temp.file_loc != C_NULL)
		{
			file.seekg(temp.file_loc * C_NODE_SIZE);
			file.read((char*) &root, sizeof(avl_tree_node));
		}
	}
	if(root.parent != C_NULL)
	{
		cout << "called" << endl;
		bal(root, file, left, right, node);//entered bal here
		cout << "exitc" << endl;
	}
	else
	{
		cout << "calledRoot" << endl;
		rootbal(root, file, left, right, node);
		cout << "exitR" << endl;
	}	
	return flag;		
}
void bal(avl_tree_node &root, fstream &file, avl_tree_node &left, avl_tree_node &right, avl_tree_node &node)
{
	int rootheight = 0;
	int otherheight = 0;
	cout << root.key_value << " in bal" << endl;
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
				cout << "rrnr" << endl;
				right.parent = root.parent;
				root.right_child = right.left_child;
				if (right.left_child != C_NULL)
				{
					left.parent = root.file_loc;
				}
				root.parent = right.file_loc;
				right.left_child = root.file_loc;
				if(node.file_loc != C_NULL)
				{
					file.seekg(node.file_loc * C_NODE_SIZE);
					file.write((char*) &node, sizeof(avl_tree_node));
				}
				if(right.parent != C_NULL)
				{
					file.seekg(right.parent * C_NODE_SIZE);
					file.read((char*) &node, sizeof(avl_tree_node));
				}
				node.right_child = right.file_loc;
				node.height = right.height + 1;
				if(node.file_loc * C_NODE_SIZE);
				{
					file.seekg(node.file_loc * C_NODE_SIZE);
					file.write((char*) &node, sizeof(avl_tree_node));
				}
				if(root.left_child != C_NULL)
				{
					file.seekg(root.left_child * C_NODE_SIZE);
					file.read((char*) &node, sizeof(avl_tree_node));
					root.height = node.height + 1;
				}
				else
				{
					root.height = left.height + 1;
				}
			}//end right right
			else//right left
			{
				cout << "rlnr" << endl;
				otherheight = node.height + 1;
				left.parent = root.parent;
				if (left.left_child != C_NULL)
				{
					file.seekg(left.left_child * C_NODE_SIZE);
					file.read((char*) &node, sizeof(avl_tree_node));
				}
				else
				{
					node.height = C_NULL;
					node.file_loc = C_NULL;
				}
				root.right_child = node.file_loc;
				node.parent = root.file_loc;
				left.left_child = root.file_loc;
				root.parent = left.file_loc;
				rootheight = node.height;
				if(node.file_loc != C_NULL)
				{
					file.seekg(node.file_loc * C_NODE_SIZE);
					file.write((char*) &node, sizeof(avl_tree_node));
				}
				if(left.right_child != C_NULL)
				{
					file.seekg(left.right_child * C_NODE_SIZE);
					file.read((char*) &node, sizeof(avl_tree_node));
				}
				else
				{	
					node.height = C_NULL;
					node.file_loc = C_NULL;
				}
				node.parent = right.file_loc;
				right.left_child = node.file_loc;
				if (otherheight < node.height + 1)
				{
					otherheight = node.height + 1;
				}
				right.height = otherheight;
				if (node.file_loc != C_NULL)
				{
					file.seekg(node.file_loc * C_NODE_SIZE);
					file.write((char*) &node, sizeof(avl_tree_node));
				}
				if (root.left_child != C_NULL)
				{
					file.seekg(root.left_child * C_NODE_SIZE);
					file.read((char*) &node, sizeof(avl_tree_node));
				}
				else
				{
					node.file_loc = C_NULL;
					node.height = C_NULL;
				}
				if (rootheight < node.height + 1)
				{
					rootheight = node.height + 1;
				}
				root.height = rootheight;
				if (left.parent != C_NULL)
				{
					file.seekg(left.parent * C_NODE_SIZE);
					file.read((char*) &node, sizeof(avl_tree_node));
				}
				if (node.right_child == root.file_loc)
				{
					node.right_child = left.file_loc;
				}
				else if(node.left_child == root.file_loc)
				{
					node.left_child = left.file_loc;
				}
				if (node.file_loc != C_NULL)
				{
					file.seekg(node.file_loc  * C_NODE_SIZE);
					file.write((char*) &node, sizeof(avl_tree_node));
				}
				right.parent = left.file_loc;
				left.right_child = right.file_loc;			
			}	
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
			if (node.height >= right.height)// left left
			{
				cout << "llnr" << endl;
				left.parent = root.parent;
				root.left_child = left.right_child;
				if(left.right_child != C_NULL)
				{
					right.parent = root.file_loc;
				}
				root.parent = left.file_loc;
				left.right_child = root.file_loc;
				if(left.left_child != C_NULL)
				{
					file.seekg(node.file_loc * C_NODE_SIZE);
					file.write((char*) &node, sizeof(avl_tree_node));
				}
				if (left.parent != C_NODE_SIZE)
				{
					file.seekg(left.parent * C_NODE_SIZE);
					file.read((char*) &node, sizeof(avl_tree_node));
				}
				node.left_child = left.file_loc;
				node.height = left.height + 1;
				if (node.file_loc != C_NULL)
				{
					file.seekg(node.file_loc * C_NODE_SIZE);
					file.write((char*) &node, sizeof(avl_tree_node));
				}
				if(root.right_child != C_NULL)
				{
					file.seekg(root.right_child * C_NODE_SIZE);
					file.read((char*) &node, sizeof(avl_tree_node));
					root.height = node.height + 1;
				}
				else
				{
					root.height = right.height + 1;
				}
			}//end left left
		
			else//left right
			{
				cout << "lrnr" << endl;
				right.parent = root.parent;
				root.parent = right.file_loc;
				otherheight = node.height + 1;
				if(right.right_child != C_NULL)
				{
					file.seekg(right.right_child * C_NODE_SIZE);
					file.read((char*) &node, sizeof(avl_tree_node));
					rootheight = node.height + 1;
				}
				else
				{
					node.file_loc = C_NULL;
					node.height = C_NULL;
					rootheight = 0;
				}
				right.right_child = root.file_loc;
				root.left_child = node.file_loc;
				node.parent = root.file_loc;
				if(node.file_loc != C_NULL)
				{
					file.seekg(node.file_loc * C_NODE_SIZE);
					file.write((char*) &node, sizeof(avl_tree_node));
				}
				if (right.left_child != C_NULL)
				{
					file.seekg(right.left_child * C_NODE_SIZE);
					file.read((char*) & node, sizeof(avl_tree_node));
				}
				else
				{
					node.file_loc = C_NULL;
					node.height = C_NULL;
				}
				if (otherheight < node.height)
				{
					otherheight = node.height;
				}
				left.height = otherheight;
				left.parent = right.file_loc;
				right.left_child = left.file_loc;
				left.right_child = node.file_loc;
				node.parent = left.file_loc;
				if (node.file_loc != C_NULL)
				{
					file.seekg(node.file_loc * C_NODE_SIZE);
					file.write((char*) &node, sizeof(avl_tree_node));
				}
				if (root.right_child != C_NULL)
				{
					file.seekg(root.right_child * C_NODE_SIZE);
					file.read((char*) &node, sizeof(avl_tree_node));
				}
				else
				{
					node.file_loc = C_NULL;
					node.height = C_NULL;
				}
				if (rootheight < node.height + 1)
				{
					rootheight = node.height + 1;
				}
				root.height = rootheight;
				if (root.height > left.height)
				{
					right.height = root.height + 1;
				}
				else
				{
					right.height = left.height + 1;
				}
				if (right.parent != C_NULL)
				{
					file.seekg(right.parent * C_NODE_SIZE);
					file.read((char*) &node, sizeof(avl_tree_node));
				}
				if(node.left_child == root.file_loc)
				{
					node.left_child = right.file_loc;
				}
				else
				{
					node.right_child = right.file_loc;
				}
				if(right.parent != C_NULL)
				{
					file.seekg(right.parent * C_NODE_SIZE);
					file.write((char*) &node, sizeof(avl_tree_node));
				}
			}
		}//end left cases
		if (right.file_loc != C_NULL)
		{
			file.seekg(right.file_loc * C_NODE_SIZE);
			file.write((char*) &right, sizeof(avl_tree_node));
		}
		if (left.file_loc != C_NULL)
		{
			file.seekg(left.file_loc * C_NODE_SIZE);
			file.write((char*) & left, sizeof(avl_tree_node));
		}
		if (root.file_loc != C_NULL)
		{
			file.seekg(root.file_loc * C_NODE_SIZE);
			file.write((char*) &root, sizeof(avl_tree_node));
		}
	}
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
	int otherheight = 0;
	int rootheight = 0;
	cout << root.key_value << " in root" << endl;
	if (root.right_child != C_NULL)
	{
		file.seekg(root.right_child * C_NODE_SIZE);
		file.read((char*) &right, sizeof(avl_tree_node));
	}
	else if (root.right_child == C_NULL)
	{
		right.height = C_NULL;
	}
	if (root.left_child != C_NULL)
	{
		file.seekg(root.left_child *C_NODE_SIZE);
		file.read((char*) &left, sizeof(avl_tree_node));
	}
	else if (root.left_child == C_NULL)
	{
		left.height = C_NULL;
	}
	if (left.height - right.height < C_NULL || left.height - right.height > 1)
	{
		if (left.height < right.height) // right case
		{	
			if (right.left_child != C_NULL)
			{
				file.seekg(right.left_child * C_NODE_SIZE);
				file.read((char*) &left, sizeof(avl_tree_node));
				cout << left.key_value << "LEFT IN RIGHT CASES!!!" << endl;
			}
			else
			{
				left.height = C_NULL;
			}
			if (right.right_child != C_NULL)
			{
				file.seekg(right.right_child * C_NODE_SIZE);
				file.read((char*) &node, sizeof(avl_tree_node));
				cout << node.key_value << "NODE IN RIGHT CASES!!!" << endl;
			}
			else
			{
				node.height = C_NULL;
			}
			if (left.height <= node.height)	//right right
			{
				cout << "rrr" << endl;
				//	root's right child becomes root
				temp = root.key_value;
				root.key_value = right.key_value;
				right.key_value = temp;
			
				root.right_child = right.right_child;
				node.parent = root.file_loc;
				root.height = node.height + 1;
				right.right_child = right.left_child;
				right.left_child = root.left_child;
				
				file.seekg(node.file_loc * C_NODE_SIZE);
				file.write((char*) &node, sizeof(avl_tree_node));
				if (root.left_child != C_NULL)
				{
					file.seekg(root.left_child * C_NODE_SIZE);
					file.read((char*) &left, sizeof(avl_tree_node));
					left.parent = right.file_loc;
					otherheight = left.height  + 1;
					file.seekg(left.file_loc * C_NODE_SIZE);
					file.write((char*) &left, sizeof(avl_tree_node));
				}
				else
				{
					otherheight = 0;
				}
				if (right.right_child != C_NULL)
				{
					file.seekg(right.right_child * C_NODE_SIZE);
					file.read((char*) &left, sizeof(avl_tree_node));
				}
				else
				{
					left.height = C_NULL;
				}
				if(otherheight < left.height + 1)
				{
					otherheight = left.height + 1;
				}
				root.left_child = right.file_loc;
				right.height = otherheight;
			}
			else if (left.height > node.height) // right left
			{
				cout << "rlr" << endl;
				temp = root.key_value;
				root.key_value = left.key_value;
				left.key_value = temp;
				
				otherheight = node.height + 1;
				left.parent = root.file_loc;
				right.left_child = left.right_child;
				if (root.left_child != C_NULL)
				{
					file.seekg(root.left_child * C_NODE_SIZE);
					file.read((char*) &node, sizeof(avl_tree_node));
					rootheight = node.height + 1;
				}
				else
				{
					node.file_loc = C_NULL;
					node.height = C_NULL;
				}
				
				node.parent = left.file_loc;
				left.right_child = left.left_child;
				left.left_child = node.file_loc;
				if(node.file_loc != C_NULL)
				{
					file.seekg(node.file_loc * C_NODE_SIZE);
					file.write((char*) &node, sizeof(avl_tree_node));
				}
				root.left_child = left.file_loc;
				if (right.left_child != C_NULL)
				{
					file.seekg(right.left_child * C_NODE_SIZE);
					file.read((char*) &node, sizeof(avl_tree_node));
				}
				else
				{
					node.file_loc = C_NULL;
				}
				node.parent = right.file_loc;
				if (otherheight < node.height + 1)
				{
					otherheight = node.height + 1;
				}
				right.height = otherheight;
				if (right.left_child != C_NULL)
				{
					file.seekg(right.left_child * C_NODE_SIZE);
					file.write((char*) &node, sizeof(avl_tree_node));
				}		
				if (left.right_child != C_NULL)
				{
					file.seekg(left.right_child * C_NODE_SIZE);
					file.read((char*) &node, sizeof(avl_tree_node));
				}
				else
				{
					node.file_loc = C_NULL;
					node.height = C_NULL;
				}
				if(rootheight < node.height + 1)
				{
					rootheight = node.height + 1;
				}
				left.height = rootheight;
				if (left.height < right.height)
				{
					root.height = right.height + 1;
				}
				else
				{
					root.height = left.height + 1;
				}
			}
		}	 	
		else if (left.height > right.height)//left cases
		{
			if(left.right_child != C_NULL)
			{
				file.seekg(left.right_child * C_NODE_SIZE);
				file.read((char*) &right, sizeof(avl_tree_node));
			}
			else
			{
				right.file_loc = C_NULL;
				right.height = C_NULL;
			}
			if (left.left_child != C_NULL)
			{
				file.seekg(left.left_child * C_NODE_SIZE);
				file.read((char*) &node, sizeof(avl_tree_node));
			}
			else
			{
				node.file_loc = C_NULL;
				node.height = C_NULL;
			}
			if (right.height <= node.height)//left left
			{
				
				cout << "llr" << endl;
				
				temp = root.key_value;
				root.key_value = left.key_value;
				left.key_value = temp;
				
				root.left_child = left.left_child;
				node.parent = root.file_loc;
				root.height = node.height + 1;
				left.left_child = left.right_child;
				if (node.file_loc != C_NULL)
				{
					file.seekg(node.file_loc * C_NODE_SIZE);
					file.write((char*) &node, sizeof(avl_tree_node));
				}
				if (root.right_child != C_NULL)
				{
					file.seekg(root.right_child * C_NODE_SIZE);
					file.read((char*) &right, sizeof(avl_tree_node));
				}
				else
				{
					right.file_loc = C_NULL;
					right.height = C_NULL;
				}
				right.parent = left.file_loc;
				left.right_child = right.file_loc;
				if (right.file_loc != C_NULL)
				{
					file.seekg(right.file_loc * C_NODE_SIZE);
					file.write((char*) &right, sizeof(avl_tree_node));
				}
				root.right_child = left.file_loc;

				if(left.right_child != C_NULL)
				{
					right.parent = left.file_loc;
					left.height = right.height + 1;			
				}
				root.right_child = left.file_loc;
				if(left.left_child != C_NULL)
				{
					file.seekg(left.left_child * C_NODE_SIZE);
					file.read((char*) &right, sizeof(avl_tree_node));
					otherheight = node.height + 1;
				}
				else
				{
					right.file_loc = C_NULL;
					otherheight = 0;
				}
				if (left.right_child != C_NULL)
				{
					file.seekg(left.right_child * C_NODE_SIZE);
					file.read((char*) &node, sizeof(avl_tree_node));
				}
				else
				{
					node.height = C_NULL;
					node.file_loc = C_NULL;
				}
				if (otherheight < node.height + 1)
				{
					otherheight = node.height + 1;
				}
				left.height = otherheight;
					
				file.seekg(root.file_loc * C_NODE_SIZE);
				file.write((char*) &root, sizeof(avl_tree_node));
				file.seekg(left.file_loc * C_NODE_SIZE);
				file.write((char*) &left, sizeof(avl_tree_node));	
				
			}
			else if (right.height > node.height) // left right
			{
				cout << "lrr" << endl;
				temp = root.key_value;
				root.key_value = right.key_value;
				right.key_value = temp;


				otherheight = node.height + 1;
				right.parent = root.file_loc;
				left.right_child = right.left_child;
				if (root.right_child != C_NULL)
				{
					file.seekg(root.right_child * C_NODE_SIZE);
					file.read((char*) &node, sizeof(avl_tree_node));
					rootheight = node.height + 1;
				}
				else
				{
					node.file_loc = C_NULL;
					node.height = C_NULL;
				}
				
				node.parent = right.file_loc;
				right.left_child = right.right_child;
				right.right_child = node.file_loc;
				if(node.file_loc != C_NULL)
				{
					file.seekg(node.file_loc * C_NODE_SIZE);
					file.write((char*) &node, sizeof(avl_tree_node));
				}
				root.right_child = right.file_loc;
				if (left.right_child != C_NULL)
				{
					file.seekg(left.right_child * C_NODE_SIZE);
					file.read((char*) &node, sizeof(avl_tree_node));
				}
				else
				{
					node.file_loc = C_NULL;
				}
				node.parent = left.file_loc;
				if (otherheight < node.height + 1)
				{
					otherheight = node.height + 1;
				}
				left.height = otherheight;
				if(right.left_child != C_NULL)
				{
					file.seekg(right.left_child * C_NODE_SIZE);
					file.read((char*) &node, sizeof(avl_tree_node));
				}
				else
				{
					node.file_loc = C_NULL;
					node.height = C_NULL;
				}
				if(rootheight < node.height + 1);
				{
					rootheight = node.height + 1;
				}
				right.height = rootheight;
				if(right.height < left.height)
				{
					root.height = left.height + 1;
				}
				else
				{
					root.height = left.height + 1;
				}
			}
		}// end left cases
		if (right.file_loc != C_NULL);
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
	}
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
