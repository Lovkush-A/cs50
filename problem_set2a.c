#include <cs50.h>
#include <stdio.h>
#include <string.h>  //for strlen()
#include <math.h>
#include <ctype.h> //for isalpha()


int count_letters(string text);
int count_words(string text);
int count_sentences(string text);
int CL_index(int l, int w, int s);


int main(void)
{
    //get sample text from user
    string text = get_string("Text: ");

    //calculate index
    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);
    int index = CL_index(letters, words, sentences);

    //print grade level of text
    if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
}


//counts numbers of letters in string
//measured using isalpha
int count_letters(string text)
{
    int length = strlen(text);
    int counter = 0;

    for (int i = 0; i < length; i++)
    {
        if (isalpha(text[i]))
        {
            counter++;
        }
    }
    return counter;
}



//counts number of words in string
//measured by counting number of spaces, then adding 1
int count_words(string text)
{
    int length = strlen(text);
    int counter = 0;

    for (int i = 0; i < length; i++)
    {
        if (text[i] == ' ')
        {
            counter++;
        }
    }
    return counter + 1; //add 1 because we have not yet counted the final word in text
}



//counts number of sentences in string
//measured by number of . ! or ? in string
int count_sentences(string text)
{
    int length = strlen(text);
    int counter = 0;

    for (int i = 0; i < length; i++)
    {
        char c = text[i];
        if (c == '.' || c == '!' || c == '?')
        {
            counter++;
        }
    }
    return counter;
}



//calculate Coleman-Liau index
//inputs are number of letters, words and sentences in text
int CL_index(int l, int w, int s)
{
    //calculate average number of letters per 100 words
    float L = (float) l / (float) w * 100 ;

    //calculate average number of sentences per 100 words
    float S = (float) s / (float) w * 100 ;

    //calculate index
    int index = round(0.0588 * L - 0.296 * S - 15.8);

    return index;
}
