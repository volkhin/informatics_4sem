#ifndef VOLKHIN_GRAPH_H
#define VOLKHIN_GRAPH_H

#include <vector>
#include <set>
#include <cassert>

class Graph {
 public:
  typedef std::set<size_t>::iterator iterator;

  Graph(size_t n = 0) {
    resize(n);
  }

  void resize(size_t size) {
    g.resize(size);
  }

  size_t size() const {
    return g.size();
  }

  void addOriented(int v1, int v2) {
    v1 = norm(v1);
    v2 = norm(v2);
    assert(0 <= v1 && v1 < (int)size());
    assert(0 <= v2 && v2 < (int)size());
    g[v1].insert(v2);
  }

  void addBidirected(int v1, int v2) {
    addOriented(v1, v2);
    addOriented(v2, v1);
  }

  void findSCC() {
    timer = 0;
    used.assign(size(), false);
    uptime.assign(size(), 0);
    stack.resize(0);
    components.resize(0);
    componentId.assign(size(), -1);

    for (size_t v = 0; v < size(); v++)
      if (!used[v])
        dfs(v);
  }

  size_t getComponentId(int v) {
    v = norm(v);
    assert(0 <= v && v < (int)size());
    return componentId[v];
  }

  std::vector<int> getComponent(size_t componentId) {
    assert(componentId < components.size());
    return components[componentId];
  }

  size_t getComponentsCount() {
    return components.size();
  }

  Graph::iterator begin(int v) {
    v = norm(v);
    assert(0 <= v && v < (int)size());
    return g[v].begin();
  }

  Graph::iterator end(int v) {
    v = norm(v);
    assert(0 <= v && v < (int)size());
    return g[v].end();
  }

 private:
  static const int INF = (int)1e9;
  std::vector<std::set<size_t> >g;
  int timer;
  std::vector<bool> used;
  std::vector<size_t> uptime;
  std::vector<size_t> stack;
  std::vector<size_t> componentId;
  std::vector<std::vector<int> > components;

  void dfs(size_t v) {
    used[v] = true;
    uptime[v] = timer++;
    stack.push_back(v);

    bool isRoot = true;

    for (Graph::iterator it = begin(v); it != end(v); ++it) {
      size_t v2 = *it;
      if (!used[v2])
        dfs(v2);
    }

    for (Graph::iterator it = begin(v); it != end(v); ++it) {
      size_t v2 = *it;
      if (uptime[v] > uptime[v2]) {
        uptime[v] = uptime[v2];
        isRoot = false;
      }
    }

    if (isRoot) {
      components.push_back(std::vector<int>());
      while (true) {
        size_t u = stack.back();
        stack.pop_back();
        componentId[u] = components.size() - 1;
        components.back().push_back(denorm(u));
        uptime[u] = INF;
        if (v == u)
          break;
      }
    }
  }

  int norm(int x) {
    if (x < 0)
      x = (size() / 2) + (~x);
    return x;
  }

  int denorm(int x) {
    if (x >= (int)size() / 2)
      x = ~(x - size()/2);
    return x;
  }
};

#endif //  VOLKHIN_GRAPH_H

