#include"utils.h"
#include<stdio.h>
#include<stdlib.h>

void errorif(bool condition, const char *msg)
{
    if (condition)
    {
        perror(msg);
        exit(EXIT_FAILURE);
    }
}
