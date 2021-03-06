/********************************************
 *Program 2 CSC 300 Data Structures\
 
 *Benjamin Kaiser and Leif Torgersen

 *CSC 300 M001 Data Structures

 *Dr. Corwin

 *Due April 7th, 2015

 *Dates written:  March 26, 2015 through April 7th, 2015

 *The program is designed to read in values from a text file
 *and insert them into an AVL tree built on a binary file.
 *A specification was that no more than 8 nodes were to be held
 *in memory at a time.  An AVL tree is a binary search tree which
 *self balances when the subtrees height difference becomes more than
 *one off each other.  

 *The program is compiled using g++.  No special instructions are needed.
 *The program is run using command line arguments.  The first parameter
 *after the file name is the input file and the second is output file.
 *You need input as a text file (.txt) containing integers and
 *the output is a binary file specified by user.  

 *No special compilations or linking instructions
 
 *BUGS:
 *Left left root case either segmentation faults or just doesn't work
 *Other cases appear to work
 *******************************************/

#include <iomanip>
#include <iostream>
#include <fstream>

using namespace std;
/*
 *
 */
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


void display(fstream &tree, avl_tree_node *curr, int pos, int indent)
{
    tree.clear();
    tree.seekg( pos * sizeof(avl_tree_node), ios::beg );
    tree.read((char *) curr, sizeof(avl_tree_node));
    if (pos != C_NULL)
    {
        avl_tree_node *curr2 = new avl_tree_node;
        curr2->key_value = curr->key_value;
        curr2->left_child = curr->left_child;
        curr2->right_child = curr->right_child;
        curr2->height = curr->height;
        curr2->parent = curr->parent;
        curr2->file_loc = curr->file_loc;
        display(tree, curr2, curr2->right_child, indent + 6);
        delete curr2;
        if (indent > 0)
            cout << setw(indent) << " ";
        if( pos != 0 )
        { 
          avl_tree_node *temp = new avl_tree_node;
          tree.seekg( curr->parent * sizeof(avl_tree_node), ios::beg );
          tree.read((char *) temp, sizeof(avl_tree_node)); 
          cout << temp-> key_value << " " << curr->key_value << endl;
          delete temp;
        }else
        {
          cout << curr->key_value << endl;
        }
        display(tree, curr, curr->left_child, indent + 6);
    }
    tree.clear();
}

// call in main  

/********************************************
 *Main
 *Main initializes all temporary nodes. 
 *It loops to read in nodes fromt text file and calls
 *insert function.  The files are opened in this function as well.
 *the paramaters are command line arguments for file opening
 ********************************************/

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
	finout.open(argv[2], ios::in | ios:: out | ios::ate | ios::trunc | ios::binary);
	//tempnode[0]  is always the one that operations are being performed on
	while (fin >> tempnode.key_value)
	{
		
		tempnode.file_loc = count;
		flag = false;
		height = false;
		insert(tempnode, finout, 0, flag, root, height, temp, left, right);//passes key value set and all other fields junk except file_loc
		count++; // this is where i start for next evaluation
/*		
		root.key_value = C_NULL;
		root.left_child = C_NULL;
		root.right_child = C_NULL;
		root.height = C_NULL;
		root.parent = C_NULL;
		root.file_loc = C_NULL;
*/
	}
avl_tree_node *give = new avl_tree_node;
  display(finout, give, 0, 0);

	finout.close();
	fin.close();

	return 0;
}
/***********************************************
 *Insert
 *This function performs much of the processing of the program.
 *It is a recursive function which steps through down to the bottom of the
 *tree and inserts the value at the appropriate spot.   
 *It also calls bal or rootbal as necessary to correctly balance the tree
 ***********************************************/
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
		if(node.file_loc != C_NULL)
		{
			file.seekg(node.file_loc * C_NODE_SIZE);
			file.read((char*) &node, sizeof(avl_tree_node));
		}
		
		if (node.file_loc != C_NULL)
		{
			file.seekg(node.file_loc * C_NODE_SIZE);
			file.read((char*) &root, sizeof(avl_tree_node));
		}
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
	
	if (node.key_value > root.key_value)
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
		
		bal(root, file, left, right, node);//entered bal here
	}
	else
	{
		
		rootbal(root, file, left, right, node);
		
	}	
	return flag;		
}
/************************************
 * Bal
 * This function should account for all nonroot balance cases.
 * It takes a root node recursively from the insert, the file, the temporary nodes
 * left and right and node which originally contained the node to be
 * inserted in the tree.  
 * 
 ************************************/
void bal(avl_tree_node &root, fstream &file, avl_tree_node &left, avl_tree_node &right, avl_tree_node &node)
{
	int rootheight = 0;
	int otherheight = 0;
	
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
/*********************************
 * Rootbal
 * This handles all root cases for the balance function
 * It is passed by reference root which is from the recursive
 * insert function.  It also is passed an fstream and left and right
 * temporary nodes which are used to manipulate pointers around
 *********************************/
void rootbal(avl_tree_node &root, fstream &file, avl_tree_node &left, avl_tree_node &right,avl_tree_node &node)
{
	int temp;
	int otherheight = 0;
	int rootheight = 0;
	
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
				
	
				
				temp = root.key_value;
				root.key_value = left.key_value;
				left.key_value = temp;
				
				root.left_child = node.file_loc;
				node.parent = root.file_loc;
				root.height = node.height + 1;
				left.left_child = left.right_child;
				left.right_child = root.right_child;
				
				rootheight = node.height + 1;
				if (node.file_loc != C_NULL)
				{
					file.seekg(node.file_loc * C_NODE_SIZE);
					file.write((char*) &node, sizeof(avl_tree_node));
				}
				if(root.right_child != C_NULL)
				{
					file.seekg(root.right_child * C_NODE_SIZE);
					file.read((char*) &node, sizeof(avl_tree_node));
					node.parent = left.file_loc;
					otherheight = node.height + 1;
					file.seekg(node.file_loc * C_NODE_SIZE);
					file.write((char*) &node, sizeof(avl_tree_node));
				}
				else
				{
					otherheight = 0;
				}
				if(right.height+1 > rootheight)
				{
					rootheight = node.height + 1;
				}
				if(otherheight < right.height + 1)
				{
					otherheight = right.height + 1;
				}
				root.right_child = left.file_loc;
				root.height = rootheight;
				left.height = otherheight;
			
			/*	if (root.right_child != C_NULL)
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
			*/	file.seekg(right.file_loc * C_NODE_SIZE);
				file.write((char*) &right, sizeof(avl_tree_node));	
				file.seekg(root.file_loc * C_NODE_SIZE);
				file.write((char*) &root, sizeof(avl_tree_node));
				file.seekg(left.file_loc * C_NODE_SIZE);
				file.write((char*) &left, sizeof(avl_tree_node));	
			}	
			else if (right.height > node.height) // left right
			{
		
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
