#include "complete_binary_tree.h"

int main() {
  CompleteBinaryTree<string> cbt;
  string s = "word";
  int n = 20;
  for(int i = 0; i < n; i++) {
    cbt.append(s);
    cout << cbt << "\n";
  }

}
