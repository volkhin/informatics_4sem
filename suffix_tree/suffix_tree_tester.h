// Copyright (c) 2011 Artem Volkhin
#ifndef SUFFIX_TREE_SUFFIX_TREE_TESTER_H_
#define SUFFIX_TREE_SUFFIX_TREE_TESTER_H_

#include <vector>
#include <cassert>
#include "./suffix_tree_tester.h"

template <class Char>
class SuffixTreeTester : public SuffixTree<Char> {
 public:
  bool contains(const std::vector<Char>& s) {
    typename SuffixTree<Char>::Position * pos =
        new typename SuffixTree<Char>::Position(this->root_->parent_edge, 0);
    bool ret = true;
    for (size_t i = 0; i < s.size(); i++) {
      if (pos->edge->end_pos == pos->offset) {
        typename SuffixTree<Char>::Node * cur_node = pos->edge->to;
        if (cur_node->edges.find(s[i]) != cur_node->edges.end()) {
          pos->edge = getEdge(cur_node, s[i]);
          pos->offset = pos->edge->start_pos + 1;
        } else {
          ret = false;
        }
      } else if (this->str_[pos->offset] == s[i]) {
        pos->offset++;
      } else {
        ret = false;
      }
    }
    delete pos;
    return ret;
  }

  bool checkForBadNodes() {
    return recursiveCheckForBadNodes(this->root_);
  }

  void dump() {
    fprintf(stderr, "\n=============== DUMP BEGINS HERE ===============\n");
    for (size_t i = 0; i < this->str_.size(); i++)
      putchar(this->str_[i]);
    putchar('\n');
    recursiveDump(this->root_, 0);
    fprintf(stderr, "================ DUMP ENDS HERE ================\n\n");
  }

 protected:
  void recursiveDump(typename SuffixTree<Char>::Node * node, int offset) {
    for (int i = 0; i < offset; i++)
      fprintf(stderr, " ");

    dump(node);
    fprintf(stderr, " ");
    typename SuffixTree<Char>::Edge * edge = node->parent_edge;
    dump(edge);
    fprintf(stderr, "\n");

    for (__typeof(node->edges.begin()) it = node->edges.begin();
         it != node->edges.end(); ++it)
      recursiveDump(it->second->to, offset + 1);
  }

  bool recursiveCheckForBadNodes(typename SuffixTree<Char>::Node * node) {
    if (node != this->root_) {
      if (node->edges.size() == 1)
        return false;
    }
    for (__typeof(node->edges.begin()) it = node->edges.begin();
         it != node->edges.end(); ++it)
      if (!recursiveCheckForBadNodes(it->second->to))
        return false;
    return true;
  }

  void dump(typename SuffixTree<Char>::Node * node) {
    fprintf(stderr, "Node(id=%d %d)", node->id,
            node->suffix_link != NULL ? node->suffix_link->id : -666);
  }

  void dump(typename SuffixTree<Char>::Edge * edge) {
    fprintf(stderr, "Edge(%p %d %d %d %d '%c')", edge, edge->from->id,
            edge->to->id, edge->start_pos, edge->end_pos, edge->first_char);
  }

  void dump(typename SuffixTree<Char>::Position * pos) {
    fprintf(stderr, "Position(%p)", pos);
    if (pos != NULL) {
      dump(pos->edge);
      fprintf(stderr, " %d", pos->offset);
    }
  }
};

#endif  // SUFFIX_TREE_SUFFIX_TREE_TESTER_H_
