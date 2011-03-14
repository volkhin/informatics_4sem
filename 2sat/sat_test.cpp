#define DEBUG
#include <cstdio>
#include <vector>
#include <gtest/gtest.h>
#include "graph.h"
#include "sat.h"

TEST(tarjan, SCC_testing) {
  Graph g(8);
  g.addOriented(0, 4);
  g.addOriented(1, 6);
  g.addOriented(4, 2);
  g.addOriented(4, 3);
  g.addOriented(4, 5);
  g.addOriented(5, 6);
  g.addOriented(6, 7);
  g.addOriented(7, 4);
  g.findSCC();

  std::vector<std::vector<int> > res;
  res.resize(5);
  res[0].push_back(0);
  res[1].push_back(1);
  res[2].push_back(2);
  res[3].push_back(3);
  for (int i = 4; i < 8; i++)
    res[4].push_back(i);

  for (size_t i = 0; i < res.size(); i++)
    for (size_t j = i; j < res.size(); j++)
      for (size_t v1 = 0; v1 < res[i].size(); v1++)
        for (size_t v2 = 0; v2 < res[j].size(); v2++) {
          if (i == j)
            EXPECT_EQ(g.getComponentId(res[i][v1]), g.getComponentId(res[j][v2]));
          else
            EXPECT_NE(g.getComponentId(res[i][v1]), g.getComponentId(res[j][v2]));
        }
}

TEST(SAT, simple_test) {
  // (a || b) && (b || !c)
  SAT sat(3);
  std::vector <bool> data(4);
  data[0] = 0;
  data[1] = 1;
  data[2] = 1;
  data[3] = 1;
  sat.addFormula(0, 1, data);
  data[0] = 1;
  data[1] = 0;
  data[2] = 1;
  data[3] = 1;
  sat.addFormula(1, 2, data);

  std::vector<bool> solution;
  bool result = sat.solve(solution);
  EXPECT_EQ(true, result);

  fprintf(stderr, "solution:\n");
  for (int i = 0; i < 3; i++)
    fprintf(stderr, "%d - %d\n", i, (int)solution[i]);

  EXPECT_EQ(true, sat.check(solution));
}

TEST(SAT, simple_test2) {
  SAT sat(3);
  std::vector <bool> data(4);
  data[0] = 0;
  data[1] = 1;
  data[2] = 1;
  data[3] = 1;
  sat.addFormula(0, 1, data);
  data[0] = 0;
  data[1] = 1;
  data[2] = 0;
  data[3] = 0;
  sat.addFormula(1, 2, data);

  std::vector<bool> solution;
  bool result = sat.solve(solution);
  EXPECT_EQ(true, result);

  fprintf(stderr, "solution:\n");
  for (int i = 0; i < 3; i++)
    fprintf(stderr, "%d - %d\n", i, (int)solution[i]);

  EXPECT_EQ(true, sat.check(solution));
}

TEST(SAT, DISABLED_large_random) {
  int n = 10000;
  int m = 100000;
  std::vector<bool> solution(n);
  for (int i = 0; i < n; i++)
    solution[i] = (bool)(rand() & 1);

  SAT sat(n);
  for (int i = 0; i < m; i++) {
    int var1 = rand() % n, var2 = rand() % n;
    std::vector <bool> data(4);
    for (int v1 = 0; v1 < 2; v1++)
      for (int v2 = 0; v2 < 2; v2++) {
        data[2*v1 + v2] = data[2*v1 + v2] | (v1 ? solution[var1]: !solution[var1]);
        data[2*v1 + v2] = data[2*v1 + v2] | (v2 ? solution[var2]: !solution[var2]);
      }
    sat.addFormula(var1, var2, data);
  }

  EXPECT_EQ(true, sat.check(solution));

  std::vector<bool> ans;
  bool result = sat.solve(ans);

  EXPECT_EQ(true, result);
  EXPECT_EQ(true, sat.check(ans));
}

