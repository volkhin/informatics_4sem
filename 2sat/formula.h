#ifndef VOLKHIN_FORMULA_H
#define VOLKHIN_FORMULA_H

#include <cstdlib>
#include <cassert>
#include <map>

class Formula {
  public:
   Formula(int x, int y) : x(x), y(y) {
   }

   void add(bool x, bool y, bool value) {
     values[std::make_pair(x, y)] = value;
   }

   bool get(bool x, bool y) {
     std::pair<bool, bool> request = std::make_pair(x, y);
     std::map<std::pair<bool,bool>, bool>::iterator it = values.find(request);
     assert(it != values.end());
     return it->second;
   }

   int getX() {
     return x;
   }

   int getY() {
     return y;
   }

  private:
   std::map<std::pair<bool, bool>, bool> values;
   int x, y;
};

#endif //  VOLKHIN_FORMULA_H

