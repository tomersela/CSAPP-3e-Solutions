#include<assert.h>

/* Return 1 when any odd bit of x equals 1; 0 otherwise.
     Assume w=32 */
  int any_odd_one(unsigned x) {
      int mask = 0xAAAAAAAA;
      return !!(x & mask);
  }

  int main() {
      assert(any_odd_one(0b010));
      assert(!any_odd_one(0b101));
      return 0;
  }