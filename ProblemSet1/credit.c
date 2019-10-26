#include<stdio.h>
#include<cs50.h>
#include<math.h>

int main()
{
    long n, temp, sum1 = 0, sum2 = 0;
    int tmp;
    n  = get_long("Number: ");
    temp = (n -n%10)/10;
    while(temp>0)
    {   tmp = (temp%10)*2; 
        if(temp%10 < 5)
            sum1 += tmp;
        else
            sum1 += tmp%10 + (tmp - tmp%10)/10;
        temp = (temp - temp%10)/10;
        sum2 += temp%10;
        temp = (temp - temp%10)/10;
    }
    sum2 += n%10;
    if((sum1 + sum2)%10 != 0)
    {
        printf("INVALID\n");
        return 0;
    }
    long first, first2;
    first = n;
    first2 = n;
    int i = 0;
    while (first>9)
    {
        first = (first - first%10)/10;
        i++;
    }
    first2 = first2 - first*pow(10,i);
    while(first2>9)
    {
        first2 = (first2 - first2%10)/10;
    }
    first2 = first2 + first*10;
    i++;
    if((first2 == 37 || first2 == 34) && i == 15 )  
        printf("AMEX\n");
    else if((first2 > 50 && first2 < 56) && i == 16)
        printf("MASTERCARD\n");
    else if(first == 4 && ( i == 13 || i == 16 ))
        printf("VISA\n"); 
    else
        printf("INVALID\n");
}
