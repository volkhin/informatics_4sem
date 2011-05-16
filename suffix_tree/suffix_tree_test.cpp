// Copyright (c) 2011 Artem Volkhin
#include <cstdio>
#include <string>
#include <vector>
#include "./suffix_tree.h"
#include "./suffix_tree_tester.h"

int main() {
  std::vector<char> string;
  unsigned int seed = 1234;
  for (int i = 0; i < 100; i++)
    string.push_back('a' + rand_r(&seed) % 26);

  SuffixTreeTester<char> suffix_tree;
  suffix_tree.addString(string);
  // suffix_tree.dump();

  // Test 1: the tree contains every substring of s
  for (size_t i = 0; i < string.size(); i++)
    for (size_t j = i + 1; j <= string.size(); j++) {
      std::vector<char> substring(string.begin() + i, string.begin() + j);
      assert(suffix_tree.contains(substring) == true);
    }
  printf("Test 1 passed.\n");

  // Test 2: there is no node with alone child (the only exception
  // is root node).
  assert(suffix_tree.checkForBadNodes());
  printf("Test 2 passed.\n");
  printf("All tests passed, it works!\n");

  return 0;
}
