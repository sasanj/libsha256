#include "tests.h"

int main(void) {
  bool result = prepare_test();
  if (result) {
    result = do_test();
    tear_down_test();
  }
  return (result? 0:1);
}
