#ifndef VOLKHIN_SAT_H
#define VOLKHIN_SAT_H

#include <cassert>
#include <vector>
#include "graph.h"
#include "formula.h"

class SAT {
 public:
  SAT(size_t n) : n(n) {
    graph.resize(2 * n);
  }

  void addFormula(int x, int y, std::vector<bool> data) {
    assert(data.size() == 4);
    Formula f(x, y);
    for (int xVal = 0; xVal < 2; xVal++)
      for (int yVal = 0; yVal < 2; yVal++) {
        f.add((bool)xVal, (bool)yVal, data[xVal*2 + yVal]);
      }
    formulas.push_back(f);
  }

  bool solve(std::vector<bool> & output) {
    buildImplications();
    graph.findSCC();
#ifdef DEBUG
  fprintf(stderr, "scc:\n");
  for (int i = 0; i < (int)n; i++) {
    fprintf(stderr, "%d - %d\n", i, (int)graph.getComponentId(i));
    fprintf(stderr, "!%d - %d\n", i, (int)graph.getComponentId(~i));
  }
#endif
    for (int i = 0; i < (int)n; i++)
      if (graph.getComponentId(i) == graph.getComponentId(~i))
        return false;
    
    output.resize(n);
    std::vector<bool> componentsColor(graph.getComponentsCount(), false);

    for (int i = 0; i < componentsColor.size(); i++) {
      const std::vector<int> & vertexes = graph.getComponent(i);
      for (int j = 0; j < (int)vertexes.size(); j++) {
        int v = vertexes[j];
        if (v >= 0)
          output[v] = componentsColor[i];
        int another = graph.getComponentId(~v);
        if (another > v)
          componentsColor[another] = !componentsColor[i];
      }
    }

#ifdef DEBUG
    fprintf(stderr, "componentsColor:\n");
    for (int i = 0; i < (int)componentsColor.size(); i++)
      fprintf(stderr, "%d - %d\n", i, (int)componentsColor[i]);
#endif

    /*
    for (int i = 0; i < (int)n; i++) {
      int c1 = graph.getComponentId(i);
      int c2 = graph.getComponentId(~i);
      if (c1 < c2)
        output[i] = true;
      else
        output[i] = false;
    }
    */
    return true;
  }

  bool check(const std::vector<bool>& solution) {
    for (size_t i = 0; i < n; i++) {
      Formula & f = formulas[i];
      int x = f.getX(), y = f.getY();
      bool value = f.get(solution[x], solution[y]);
#ifdef DEBUG
      fprintf(stderr, "x = %d, y = %d, f.get(%d, %d) = %d\n", 
              x, y, (int)solution[x], (int)solution[y], (int)value);
#endif
      if (!value)
        return false;
    }
    return true;
  }

 private:
  size_t n;
  std::vector<Formula> formulas;
  Graph graph;
  
  void buildImplications() {
    for (size_t i = 0; i < formulas.size(); i++) {
      Formula & formula = formulas[i];
      int vars[2];
      vars[0] = formula.getX();
      vars[1] = formula.getY();

      for (int pos = 0; pos < 2; pos++)
        for (int val = 0; val < 2; val++) {
          int v[2];
          v[pos] = val;
          v[1-pos] = 0;
          bool val1 = formula.get(v[0], v[1]);
#ifdef DEBUG
          fprintf(stderr, "formula.get(%d, %d) = %d\n", v[0], v[1], (int)val1);
#endif
          v[1-pos] = 1;
          bool val2 = formula.get(v[0], v[1]);
#ifdef DEBUG
          fprintf(stderr, "formula.get(%d, %d) = %d\n", v[0], v[1], (int)val2);
#endif
          if (val1 + val2 == 1) {
            if (val1 == 1)
              v[1-pos] = 0;

            addImplication(vars[pos], v[pos], vars[1-pos], v[1-pos]);
          }
        }
    }
  }

  void addImplication(int var1, bool val1, int var2, bool val2) {
#ifdef DEBUG
    fprintf(stderr, "Adding implication %s%d -> %s%d\n", 
            val1 ? "" : "!", var1, val2 ? "" : "!", var2);
#endif
    graph.addOriented(val1 ? var1 : ~var1, val2 ? var2 : ~var2);
  }

};

#endif //  VOLKHIN_SAT_H
