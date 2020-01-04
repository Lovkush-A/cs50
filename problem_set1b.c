#include <cs50.h>
#include <stdio.h>

void p_space(int n);
void p_hash(int n);
void p_row(int n, int j);

int main(void)
{
    //obtain height from user
    int n = 0;
    while (n < 1 || n > 8)
    {
        n = get_int("Height: ");
    }
    
    //print pyramid
    for (int i = 1; i <= n; i++)
    {
        p_row(n, i);
    }    
}


//function that prints n spaces
void p_space(int n)
{
    for (int i = 0; i < n; i++)
    {
        printf(" ");
    }
}


//function that prints n hashes
void p_hash(int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("#");
    }
}


//function that prints jth row of n-size pyramind
//and prints new line
void p_row(int n, int j)
{
    p_space(n - j);
    p_hash(j);
    printf("  ");
    p_hash(j);
    printf("\n");
}


