#include <stdio.h>
#include <cs50.h>
#include <math.h>

//step 1: checksum
//step 2: check conditions for each type of card separately.

int length(long n);
int final_digit(long n);
int kth_digit(long n, int k);
int sum_digits_2k(int k);
int checksum(long n);
bool checkamex(long n);
bool checkmastercard(long n);
bool checkvisa(long n);


int main(void)
{
    long n = get_long("Number: ");
    if (checksum(n) != 0)
    {
        printf("INVALID\n");
    }
    else if (checkamex(n))
    {
        printf("AMEX\n");
    }
    else if (checkmastercard(n))
    {
        printf("MASTERCARD\n");
    }
    else if (checkvisa(n))
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
    }
}


//length of n
int length(long n)
{
    return ceil(log10(n));
}

//function creates final digit of integer
int final_digit(long n)
{
    return n % 10;
}

//function obtains kth digit *from the right*
int kth_digit(long n, int k)
{
    if (k == 1)
    {
        return final_digit(n);
    }
    else
    {
        //remove final digit from n and do recursion
        n = (n - final_digit(n)) / 10;
        k = k - 1;
        return kth_digit(n, k);
    }
}

//sum of digits in 2k, for k=0 to 9. no checking for size of n
int sum_digits_2k(int k)
{
    if (k < 5)
    {
        return 2 * k;
    } 
    else if (k == 5)
    {
        return 1;
    }
    else
    {
        return 2 + sum_digits_2k(k - 1);
    }   
}

//checksum card number
int checksum(long n)
{
    int sum = 0;
    for (int i = 1; i <= length(n); i++)
    {
        //for even i, add sum of digits in twice ith digit of n
        if (i % 2 == 0)
        {
            sum = sum + sum_digits_2k(kth_digit(n, i));
        }
        
        //for odd i, add ith digit of n
        else
        {
            sum = sum + kth_digit(n, i);
        } 
    }
    printf("checksum equals %d\n", sum);
    return sum % 10;
}

//check if card satisfies amex conditions
bool checkamex(long n)
{
    //determine first two digits
    int l = length(n);
    int first_digits = 10 * kth_digit(n, l) + kth_digit(n, l - 1);
    printf("amex first digits %d\n", first_digits);
    
    if (length(n) != 15)
    {
        return false;
    }
    else if (first_digits == 34 || first_digits == 37)
    {
        return true;
    } 
    else
    {
        return false;
    }  
}

//check if card satisfies mastercard conditions
bool checkmastercard(long n)
{
    //determine first two digits
    int l = length(n);
    int first_digits = 10 * kth_digit(n, l) + kth_digit(n, l - 1);
    printf("mastercard first digits %d\n", first_digits);
    
    if (length(n) != 16)
    {
        return false;
    }
    else if (first_digits < 51 || first_digits > 55)
    {
        return false;
    }
    else
    {
        return true;
    }
}

//check if card satisfies visa conditions
bool checkvisa(long n)
{
    //determine first two digits
    int l = length(n);
    int first_digits = kth_digit(n, l);
    printf("visa first digits %d\n", first_digits);
    
    if (length(n) != 13 && length(n) != 16)
    {
        return false;
    }
    else if (first_digits != 4)
    {
        return false;
    }
    else
    {
        return true;
    }
}

