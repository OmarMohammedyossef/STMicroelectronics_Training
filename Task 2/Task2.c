#include <stdio.h>
#include <stdlib.h>

int main()
{
    char input [30];
    char exit [10]="exit";
    int i=0;
    char count=0;
    while(1)
    {
        printf("Sama 3likooooo >");
       fgets(input,30,stdin);
        for(i=0;i<4;i++)
        {
           if(input[i] == exit[i])
           {
               count++;
           }
           else{}
        }
        if(count==4)
        {
            printf("Etla3 Bara \n");
            break;
        }
        else
        {
            count=0;
        }
        printf("%s \n",input);
    }
    return 0;

}
