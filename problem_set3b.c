#include <stdio.h>
#include <string.h>
#include <cs50.h>
#include <ctype.h>

int valid_key(int argc, string argv[]); //checks if input is valid
string cipher(string text, string key); //encrypts text using substitution cipher
char cipher_char(char c, string key); //encrypts single character using substitution cipher


int main(int argc, string argv[])
{
    int valid = valid_key(argc, argv);
    if (valid != 0)
    {
        return valid;
    }

    string key = argv[1];

    string plaintext = get_string("plaintext:  ");
    string ciphertext = cipher(plaintext, key);

    printf("ciphertext: %s\n", ciphertext);
}



//checks if user inputs is valid
int valid_key(int argc, string argv[])
{
    //check one and only one key has been inputted
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    string key = argv[1];
    int length = strlen(key);

    //check length of key
    if (length != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    //check there are only alphabetical characters
    for (int i = 0; i < length; i++)
    {
        if (!(isalpha(key[i])))
        {
            printf("Key must only contain letters.\n");
            return 1;
        }
    }


    //check no repeating characters
    for (int i = 0; i < length; i++)
    {
        for (int j = i + 1; j < length; j++)
        {
            if (tolower(key[i]) == tolower(key[j]))
            {
                printf("Key must not contain any repeating letters.\n");
                return 1;
            }
        }
    }

    return 0;
}



//encrypts text using substitution cipher
string cipher(string text, string key)
{
    int length = strlen(text);
    for (int i = 0; i < length; i++)
    {
        text[i] = cipher_char(text[i], key);
    }

    return text;
}



//encrypts single character using substitution cipher
char cipher_char(char c, string key)
{
    if (isupper(c))
    {
        c = toupper(key[(int) c - 65]); //A is 65 in ascii, so subtract 65.
    }
    if (islower(c))
    {
        c = tolower(key[(int) c - 97]); //a is 97 in ascii, so subtract 97.
    }
    return c;
}