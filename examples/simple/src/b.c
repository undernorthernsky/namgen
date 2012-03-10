#include <stdio.h>
#include <math.h>
#include "a.h"
#include "c.h"

#ifndef FOO
#error not enough FOO
#endif
int main()
{
   printf("%f\n", floor(bar(42)));
   return(foo(1));
}
