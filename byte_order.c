#include <stdio.h>

union Data
{
    int a;
    char b;
}data;

int main()
{
    data.a = 1;
    
    if(data.b == 1)
    {
        printf("little endian.\n");
    }
    else
    {
        printf("big endian.\n");
    }
    return 0;
}
