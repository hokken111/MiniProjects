#ifndef __BIN_TREE_H__
#define __BIN_TREE_H__
#include <iostream>

////////////////////////////////////////////////////////////////////////////////
//This type has been defined to refer to elements in the CompleteBinaryTree
//type. Elements in the CompleteBinaryTree should only be referred to through
//CBTNODE types. 
typedef int CBTNODE;

const CBTNODE CBTNODE_DNE = -1;

typedef int ERRTYPE;


template <typename T>
class CompleteBinaryTree {
public:
  //Initializes an empty binary tree
  CompleteBinaryTree(int initialSize = 10);

  //Destructor
  ~CompleteBinaryTree();

  //Swaps the elements at locations node1 and node2 in the CBT. 
  ERRTYPE swap(CBTNODE node1, CBTNODE node2);

  //Returns a CBTNODE referring to the root of the CompleteBinaryTree.
  CBTNODE root();

  //Returns a CBTNODE referring to the last leaf. 
  CBTNODE lastLeaf();

  //Returns a CBTNODE referring to node's left child. Returns CBTNODE_DNE
  //if node does not have a left child.
  CBTNODE leftChild(CBTNODE node);

  //Returns a CBTNODE referring to node's right child. Returns CBTNODE_DNE
  //if node does not have a right child.
  CBTNODE rightChild(CBTNODE node);

  //Returns a CBTNODE referring to node's parent node. Returns CBTNODE_DNE
  //if node does not have a parent.
  CBTNODE parent(CBTNODE node);

  //Returns true if CBTNODE is a valid node in the CBT, false otherwise.
  bool valid(CBTNODE node);
  
  //Returns the element of the CBT pointed to by node. Behavior is undefined
  //if node does not exist. 
  T element(CBTNODE node);

  //Adds newElt as the newest leaf. 
  void append(T newElt);

  //Removes the last leaf and returns its value.
  T removeLast();

  void print(std::ostream &out = std::cout);

  //Clears the tree, erasing all of the elements within. If the elements 
  //are dynamically allocated pointers, this method should not be used, 
  //because it will leak memory. Instead, each element should be removed 
  //manually through removeLast(). 
  void clear(int newSize = 10);

  //Returns true if the tree is empty, false if not.
  bool empty();

  //Returns the number of elements in the tree.
  int size();
  
private:
  T *data;
  int numElts, maxElts;
};

template <typename T>
std::ostream &operator<<(std::ostream &out, CompleteBinaryTree<T> &cbt);

#include "complete_binary_tree.cpp"

#endif /* __BIN_TREE_H__ */

//This class represents a complete binary tree. A complete binary
//tree is a binary tree with the following two requirements:
// - All of its leaves are on levels m or m+1
// - The lowest row has its leaves filled in from left to right.
//
//For example, If the method insert(5) is called on the left tree
//below, then the result would look like the right tree below
//
//             1                      1
//           /   \                  /   \
//          2     3   insert(5)    2     3
//         /           ------>    / \
//        4                      4   5
//
//And a further call of insert(6) would yield
//
//             1     
//           /   \   
//          2     3  
//         / \   /     
//        4   5 6 
//     
