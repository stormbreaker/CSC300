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
avl_tree_node *give = new avl_tree_node;
  display(binFile, give, 0, 0);
