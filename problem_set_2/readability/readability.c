#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

// output Grade 16+
// output Before Grade 1
//  output Grade X

// prototype
float cal_letters(string text);
float cal_words(string text);
float cal_sentence(string text);
int cal_index(float letters, float words, float sentences);

int main(void)
{
    string text = get_string("Text: ");
    float letters = cal_letters(text);
    float words = cal_words(text);
    float sentences = cal_sentence(text);
    // printf("%s\n", text);
    // printf("%f\n",letters);
    // printf("%f\n",words);
    // printf("%f\n",sentences);
    int index = cal_index(letters, words, sentences);
    // printf("%i\n",index);
    if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (index <= 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
}

// count letters
float cal_letters(string text)
{
    float letters = 0;
    for (int i = 0, length = strlen(text); i < length; i++)
    {
        if (isalpha(text[i]))
        {
            letters += 1;
        }
    }

    return letters;
    // L is average number of letters per 100 words
    // S is the average number of sentences per word
    // index = 0.0588 * L - 0.296 * S - 15.8
}

// count words
float cal_words(string text)
{
    float words = 1;
    for (int i = 0, length = strlen(text); i < length; i++)
    {
        if (isblank(text[i]))
        {
            words += 1;
        }
    }
    return words;
}

// count sentences
float cal_sentence(string text)
{
    float sentence = 0;
    for (int i = 0, length = strlen(text); i < length; i++)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            sentence += 1;
        }
    }
    return sentence;
}

// count index
int cal_index(float letters, float words, float sentences)
{
    float L = (letters / words) * 100;
    float S = (sentences / words) * 100;
    float index = (0.0588 * L) - (0.296 * S) - 15.8;
    // printf("%f\n", L);
    // printf("%f\n", S);
    // printf("%f\n", index);
    return round(index);
}
