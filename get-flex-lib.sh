#!/bin/sh

res=FLEXLIB-NOT-FOUND
ld -ll 1>/dev/null 2>/dev/null
if [ $? -eq 0 ]; then
    res=l;
else
    ld -lfl 1>/dev/null 2>/dev/null
    if [ $? -eq 0 ]; then
        res=fl
    fi
fi
rm -f a.out
echo $res
