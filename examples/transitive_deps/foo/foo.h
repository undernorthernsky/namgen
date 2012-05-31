#ifndef FOO_H
#define FOO_H

 /* transitive header dependency test
  *   => everyone that includes *this* file also needs bar.h
  */
#include "bar.h"

int foo(int y);
#endif
