#include "complete_binary_tree.h"
#include <cassert>

#include <iostream>
using namespace std;

template <typename T>
CompleteBinaryTree<T>::CompleteBinaryTree(int initialSize) {
  data = new T[initialSize];
  maxElts = initialSize;
  numElts = 0;
}

template <typename T>
CompleteBinaryTree<T>::~CompleteBinaryTree() {
  delete [] data;
}

template <typename T>
ERRTYPE CompleteBinaryTree<T>::swap(CBTNODE node1, CBTNODE node2) {
  if(node1 >= numElts || node2 >= numElts ||
     node1 < 0 || node2 < 0)
    return 1;

  T tmp = data[node1];
  data[node1] = data[node2];
  data[node2] = tmp;
  return 0;
}

template <typename T>
CBTNODE CompleteBinaryTree<T>::root() {
  if(numElts)
    return 0;
  return CBTNODE_DNE;
}

template <typename T>
CBTNODE CompleteBinaryTree<T>::lastLeaf() {
  if(numElts)
    return numElts-1;
  return CBTNODE_DNE;
}

template <typename T>
CBTNODE CompleteBinaryTree<T>::leftChild(CBTNODE node) {
  if(node == CBTNODE_DNE)
    return CBTNODE_DNE;
  CBTNODE result = node*2+1;
  if(result < numElts)
    return result;
  return CBTNODE_DNE;
}

template <typename T>
CBTNODE CompleteBinaryTree<T>::rightChild(CBTNODE node) {
  if(node == CBTNODE_DNE)
    return CBTNODE_DNE;
  CBTNODE result = node*2+2;
  if(result < numElts)
    return result;
  return CBTNODE_DNE;
}

template <typename T>
CBTNODE CompleteBinaryTree<T>::parent(CBTNODE node) {
  if(node == CBTNODE_DNE || !node) 
    return CBTNODE_DNE;
  return (node-1)/2;
}

template <typename T>
bool CompleteBinaryTree<T>::valid(CBTNODE node) {
  return node >= 0 && node < numElts;
}

template <typename T>
T CompleteBinaryTree<T>::element(CBTNODE node) {
  assert(node >= 0);
  assert(node < numElts);
  return data[node];
}

template <typename T>
void CompleteBinaryTree<T>::append(T newElt) {
  if(numElts >= maxElts) {
    maxElts *= 2;
    T *newData = new T[maxElts];
    for(int i = 0; i < maxElts; i++) {
      newData[i] = data[i];
    }
    delete [] data;
    data = newData;
  }
  data[numElts] = newElt;
  numElts++;
}

template <typename T>
T CompleteBinaryTree<T>::removeLast() {
  assert(numElts);
  T result = data[numElts-1];
  T *newData = new T[numElts];
  for(int i = 0; i < numElts; i++) 
    newData[i] = data[i];
  delete [] data;
  data = newData;
  maxElts = numElts;
  numElts--;
  return result;
}

template <typename T>
void CompleteBinaryTree<T>::print(ostream &out) {
  for (int i = 0; i < numElts; i++) 
    out << "(" << data[i] << ") ";
  for (int i = numElts; i < maxElts; i++)
    cout << "_ ";
}

template <typename T>
void CompleteBinaryTree<T>::clear(int newSize) {
  delete [] data;
  data = new T[newSize];
  maxElts = newSize;
  numElts = 0;
}

template <typename T>
ostream &operator<<(ostream &out, CompleteBinaryTree<T> &cbt) {
  cbt.print(out);
  return out;
}

template <typename T>
bool CompleteBinaryTree<T>::empty() {
  return numElts == 0;
}

template <typename T>
int CompleteBinaryTree<T>::size() {
  return numElts;
}

