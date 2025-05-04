#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// prototype
bool check_argument(string arg);
char plain_to_cypher(char plain, int key);

// arg count and arg v to
int main(int argc, string argv[])
{
    // checks if there is only 1 input argument
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    // if  arg is all digits, proceed
    string key = argv[1];
    if (!check_argument(argv[1]))
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    int k = atoi(key);
    string plaintext = get_string("plaintext:  ");
    printf("ciphertext: ");

    // convert plaintext to cypher
    for (int i = 0, length = strlen(plaintext); i < length; i++)
    {
        char encrypted_char = plain_to_cypher(plaintext[i], k);
        printf("%c", encrypted_char);
    }

    printf("\n");
    // checks if the iput argument
    return 0;
}

// function to check the arguement
bool check_argument(string arg)
{
    for (int i = 0, length = strlen(arg); i < length; i++)
    {
        if (!isdigit(arg[i]))
        {
            return false; // Found a non-digit character
        }
    }
    return true;
}

// function for text to cypher
char plain_to_cypher(char plain, int key)
{
    // check for letter or alphabet
    if (isalpha(plain))
    {
        char base = isupper(plain) ? 'A' : 'a';
        return (char) (((plain - base + key) % 26) + base);
    }
    else
    {
        return plain;
    }
}
// pseudocode
// take in non-negative int as argument (k )
// if no arg or more than one provided, than return error
// return error value of 1
