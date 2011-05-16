// Copyright (c) 2011 Artem Volkhin
#ifndef SUFFIX_TREE_SUFFIX_TREE_H_
#define SUFFIX_TREE_SUFFIX_TREE_H_

#include <cstdlib>
#include <vector>
#include <map>
#include <cassert>
#include <climits>

template <class Char>
class SuffixTree {
 public:
  SuffixTree() {
    dummy_ = new Node(NULL, NULL);
    root_ = new Node(dummy_, NULL);
    dummy_->parent_edge = new Edge(dummy_, dummy_, -1, -1, 0);
    root_->parent_edge = new Edge(dummy_, root_, -1, 0, 0);
    cur_ = new Position(root_->parent_edge, 0);
    size_ = 0;
  }

  ~SuffixTree() {
    removeNode(root_);
    removeNode(dummy_);
    delete cur_;
  }

  void addString(const std::vector<Char>& s) {
    for (size_t i = 0; i < s.size(); i++) {
      // dump();
      addChar(s[i]);
    }
  }

 public:
  struct Node;
  struct Edge;
  struct Position;

  static const int INF = INT_MAX / 10;

  std::vector<Char> str_;
  Node *root_, *dummy_;
  Position * cur_;
  int size_;

  void removeNode(Node * node) {
    if (node == NULL)
      return;
    for (__typeof(node->edges.begin()) it = node->edges.begin();
         it != node->edges.end(); ++it) {
      removeNode(it->second->to);
    }
    delete node->parent_edge;
    delete node;
  }

  void addChar(Char ch) {
    for (;;) {
      if (cur_->offset != cur_->edge->end_pos) {
        // We are on the edge...
        if (str_[cur_->offset] == ch) {
          // Next char on the edge is equal to 'ch'
          cur_->offset++;
          break;
        } else {
          // Next char is not equal to 'ch', we have to split the edge
          Node * new_node = splitEdge(cur_);
          cur_->edge = new_node->parent_edge;
        }
      }

      // We are in the node...
      Node * cur_node = cur_->edge->to;
      if (cur_node->edges.find(ch) == cur_node->edges.end()) {
        // ... there is no edge by 'ch', we need to add new edge
        Node * new_node = new Node(NULL, NULL);
        Edge * new_edge = new Edge(cur_node, new_node, size_, INF, ch);
        new_node->parent_edge = new_edge;
        cur_node->edges[ch] = new_edge;
        if (cur_->edge->from == dummy_)
          break;
        Position * new_cur = getSuffixLink(cur_);
        delete cur_;
        cur_ = new_cur;
      } else {
        // ... there is an edge by 'ch', go on it
        assert(cur_->edge->end_pos == cur_->offset);
        cur_->edge = getEdge(cur_node, ch);
        // assert(cur_->edge->start_pos == cur_->offset);
        cur_->offset = cur_->edge->start_pos + 1;
        break;
      }
    }
    str_.push_back(ch);
    size_++;
  }

  Position * getSuffixLink(Position * position) {
    Node * cur_node = position->edge->from->suffix_link;
    if (cur_node == NULL)
      return position;
    int cur_offset = position->edge->start_pos;
    for (;;) {
      assert(cur_offset < size_);
      Edge * cur_edge = getEdge(cur_node, str_[cur_offset]);
      cur_offset += cur_edge->end_pos - cur_edge->start_pos;
      if (cur_offset >= position->offset) {
        return new Position(cur_edge, position->offset - cur_offset +
                            cur_edge->end_pos);
      }
      cur_node = cur_edge->to;
    }
  }

  Node* splitEdge(Position * position) {
    Edge * cur_edge = position->edge;
    int cur_offset = position->offset;
    if (cur_edge->end_pos == cur_offset)
      return cur_edge->to;
    Node * new_node = new Node(NULL, cur_edge);
    Edge * new_edge = new Edge(new_node, cur_edge->to, cur_offset,
                               cur_edge->end_pos, str_[cur_offset]);
    cur_edge->to->parent_edge = new_edge;
    cur_edge->end_pos = position->offset;
    new_node->edges[str_[cur_offset]] = new_edge;
    cur_edge->end_pos = cur_offset;
    cur_edge->to  = new_node;
    assert(cur_edge->end_pos == new_edge->start_pos);
    Position * new_pos = getSuffixLink(position);
    new_node->suffix_link = splitEdge(new_pos);
    delete new_pos;
    return new_node;
  }

  Edge* getEdge(Node * node, const Char & ch) {
    if (node == dummy_)
      return root_->parent_edge;
    assert(node->edges.find(ch) != node->edges.end());
    if (node->edges.find(ch) == node->edges.end())
      node->edges[ch] = new Edge(node, NULL, -1, 0, 0);
    return node->edges[ch];
  }
};

template<class Char>
struct SuffixTree<Char>::Node {
  static int nextId;
  int id;
  Node * suffix_link;
  std::map<Char, Edge*> edges;
  Edge * parent_edge;
  Node(Node * suffix_link, Edge * parent_edge) : id(nextId++),
    suffix_link(suffix_link), parent_edge(parent_edge) {
    }
};
template<class Char>
int SuffixTree<Char>::Node::nextId = 0;

template <class Char>
struct SuffixTree<Char>::Edge {
  Node * from, * to;
  int start_pos, end_pos;
  Char first_char;
  Edge(Node * from,
       Node * to,
       int start_pos,
       int end_pos,
       const Char& first_char) :
      from(from),
      to(to),
      start_pos(start_pos),
      end_pos(end_pos),
      first_char(first_char)
  {}
};

template <class Char>
struct SuffixTree<Char>::Position {
  Edge * edge;
  int offset;
  Position(Edge * edge, int offset) : edge(edge), offset(offset) {}
};

#endif  // SUFFIX_TREE_SUFFIX_TREE_H_
