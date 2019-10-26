#include <stdio.h>
#include<cs50.h>
int main(void)
{
    char name[50];
    printf("What is your name?\n");
    scanf("%s",name);
    printf("hello, %s\n", name);
}
