#ifndef VOLKHIN_SAT_H
#define VOLKHIN_SAT_H

#include <cassert>
#include <vector>
#include <string>
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

  void addFormula(int x, int y, std::string stringData) {
    assert(stringData.size() == 4);
    std::vector<bool> data(4);
    for (size_t i = 0; i < 4; i++)
      data[i] = (bool)(stringData[i] - '0');
    addFormula(x, y, data);
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
    output.resize(0);
    for (int i = 0; i < (int)n; i++)
      if (graph.getComponentId(i) == graph.getComponentId(~i))
        return false;

    output.resize(n);
    /*
       std::vector<bool> componentsColor(graph.getComponentsCount(), false);

       for (int i = 0; i < (int)componentsColor.size(); i++) {
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
*/

    for (int i = 0; i < (int)n; i++) {
      int c1 = graph.getComponentId(i);
      int c2 = graph.getComponentId(~i);
      if (c1 < c2)
        output[i] = true;
      else
        output[i] = false;
    }

    return true;
  }

  bool check(const std::vector<bool>& solution) {
#ifdef DEBUG
    fprintf(stderr, "Starting check...\n");
#endif

    for (size_t i = 0; i < formulas.size(); i++) {
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
      int x = formula.getX();
      int y = formula.getY();
      for (int val1 = 0; val1 < 2; val1++) {
        for (int val2 = 0; val2 < 2; val2++) {
          if (formula.get(val1, 1 - val2) == false)
            addImplication(x, val1, y, val2);
          if (formula.get(1 - val1, val2) == false)
            addImplication(y, val2, x, val1);
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
