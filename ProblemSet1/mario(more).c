#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int n;
    do
    {
        printf("Height: ");
        scanf("%d",&n);
    }while(!(n>0 && n<9));
        
    
    for (int i=1; i<=n; i++)
    {   
        for(int j=0; j < n-i; j++)
            printf(" ");
        for(int j=0; j < i; j++)
            printf("#");
        printf("  ");
        for(int j=0; j < i; j++)
            printf("#");
        printf("\n");
    }
}
