#include "math.h"
#include "c.h"

#ifdef FOO
#error no FOO for me
#endif

float bar(int k)
{
   return sqrt(k);
}
