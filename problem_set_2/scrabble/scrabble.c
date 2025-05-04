#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>


// prototypes
int string_to_scores(string word);

// declare scores list
int scores[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int main(void)
{
    // ask for player input
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");
    // convert strings to scores

    int score1 = string_to_scores(word1);
    int score2 = string_to_scores(word2);

    // compare the scores
    if (score1 < score2)
    {
        printf("Player 2 wins!\n");
    }
    else if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}

// function to take in a string and convert to scores
// checks for upper and lower case
int string_to_scores(string word)
{
    int score = 0;
    for (int i = 0, length = strlen(word); i < length; i++)
    {
        if (isupper(word[i]))
        {
            score += scores[word[i] - 'A'];
        }
        else if (islower(word[i]))
        {
            score += scores[word[i] - 'a'];
        }
    }
    return score;
}
