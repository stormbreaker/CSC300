////////////////////////////////////////////////////////////////////
//     Binary Search Tree File Print for Debugging                //
//     Modified for Assignment 2, Spring 2015                     //
////////////////////////////////////////////////////////////////////

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
using namespace std;

typedef unsigned int t_key;
typedef int t_file_pointer;  // signed to allow -1 for NULL
const int C_NULL = -1;

const int DEBUG = true;
struct avl_tree_node
  {
  int  key_value;
  int  left_child;    // child file address
                      // [record index, not byte address]
  int  right_child;
  int  height;        // 0 if leaf
  int  parent;        // C_NULL if root
  int  file_loc;      // address on file (record index)
  };

const int C_NODE_SIZE = sizeof(avl_tree_node);
int inorder_last_print;
int num_printed = 0;

////////////////////////////////////////////////////////////////////

void binread_tree(fstream &inf,
  avl_tree_node & node_in, long location)
  {
  inf.clear();
  inf.seekg(location * C_NODE_SIZE);
  inf.read((char *) &node_in, C_NODE_SIZE);
  }

////////////////////////////////////////////////////////////////////

void binwrite_tree(fstream &inf,
  avl_tree_node record_out, long location)
  {
  inf.clear();
  inf.seekp(location * C_NODE_SIZE);
  inf.write((char *) &record_out, C_NODE_SIZE);
  }


////////////////////////////////////////////////////////////////////

void inorder(fstream &treefile, int  root_loc)
  {
  avl_tree_node root;

  if (root_loc == C_NULL)
    return;

  binread_tree(treefile, root, root_loc);
  if (treefile.fail())
    {
    cerr << "Error in inorder, root_loc = " << root_loc << endl;
    exit(1);
    }
 
  inorder(treefile, root . left_child);
  if (DEBUG)
    {
    cout << root . key_value << "  ";
    num_printed ++;
    if (num_printed % 10 == 0)
      cout << endl;
    if (inorder_last_print >= root . key_value)
      {
      cerr << "error in inorder, quitting" << endl;
      exit(12);
      }
    inorder_last_print = root . key_value;
    }
  inorder(treefile, root . right_child);

  } // inorder

////////////////////////////////////////////////////////////////////

bool check_order_recursive(fstream &treefile, 
  int  root_loc, int previous)
  {
  avl_tree_node root;
  avl_tree_node parent;

  if (root_loc == C_NULL)
    return true;

  binread_tree(treefile, root, root_loc);
  if (treefile.fail())
    {
    cerr << "Error in inorder, root_loc = " << root_loc << endl;
    exit(1);
    }

  // check parent pointers as well
  if (root . parent != C_NULL)
    {
    binread_tree(treefile, parent, root . parent);
    if ((parent . left_child != root_loc)
      && (parent . right_child != root_loc))
      {
      cerr << "Error in parent pointers" << endl;
      cerr << "   " << root . key_value << " says parent is "
           << root . parent
           << "(" << parent . key_value << ")" << endl;
      exit (42);
      }
    }

 
  if (!check_order_recursive(treefile, root . left_child, previous))
    return false;
  if (root . key_value <= previous)
    {
    cout << "Error in order " << previous << " before " 
         << root . key_value;
    return false;
    }
  return check_order_recursive(treefile, root . right_child, 
    root . key_value);

  } // inorder

////////////////////////////////////////////////////////////////////

bool check_order(fstream &treefile)
  {
  return check_order_recursive(treefile, 0, -99999);
  }


////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
  {
  avl_tree_node current;
  int          record_count;
  fstream      treefile;

  treefile.open(argv[1], ios::binary | ios::in);
  if (treefile.fail())
    {
    cout << "Unable to open input file" << endl;
    exit(1);
    }

  cout << "double size is " << sizeof(double) << endl;
  cout << "int size is " << sizeof(int) << endl;
  cout << "Record size is " << C_NODE_SIZE << endl;


  record_count = 0;
  cout << "              ";
  cout << " key   ";
  cout << " left  ";
  cout << " right ";
  cout << " parent ";
  cout << " height ";
  cout << " file loc ";
  cout << endl;
  while (true)
    {
    treefile . read((char *) &current, C_NODE_SIZE);
    if (treefile . fail())
      break;
    cout << setw(7);
    cout << "record " << record_count << "  ";
    cout << setw(9);
    cout << current . key_value << "  ";
    cout . setf(ios::showpoint | ios::fixed);
    cout << setw(6);
    cout << current . left_child << "  ";
    cout << setw(6);
    cout << current . right_child << "  ";
    cout << setw(6);
    cout << current . parent << "  ";
    cout << setw(6);
    cout << current . height << "  ";
    cout << setw(6);
    cout << current . file_loc << "  ";
    cout << endl;
    record_count ++;
    }
  cout << "Inorder traversal:  " << endl;
  inorder_last_print = -99999;
  inorder(treefile, 0);
  cout << endl;
  cout << num_printed << " values printed" << endl;
  if(check_order(treefile))
    cout << "Tree values are in order" << endl;
  else
    cout << "ERROR:  Tree values are NOT in order" << endl;
  treefile.close();
  }

////////////////////////////////////////////////////////////////////
//          End of binary search tree program                     //
////////////////////////////////////////////////////////////////////
