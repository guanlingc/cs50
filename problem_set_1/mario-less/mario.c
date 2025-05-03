// DONE get height from user
// print height rows and height columns eg X = X rows and X columns
// for each row, X-1 spaces, +1 hashes
// hases according to user input

// reprompt user if the input is not int or less than zero

// import modules
#include <cs50.h>
#include <stdio.h>

// prototypes
void print_row(int times);
void print_space(int height);
void print_hash(int height);

int main(void)
{
    // ask for height, save as int
    // declare the user_input
    int user_height;
    do
    {
        user_height = get_int("Height: ");
    }
    while (user_height < 1);

    // make a loop where each loop prints a row
    for (int i = 1; i < (user_height + 1); i++)
    {
        int space_counter = user_height - i;
        int hash_counter = i;
        print_space(space_counter);
        print_hash(hash_counter);
        printf("\n");
    }
}

// function to print space (if input = 5, print 4 spaces)
void print_space(int loop_index)
{
    for (int i = 0; i < loop_index; i++)
    {
        printf(" ");
    }
}

// function to print hash (if input = 5, print 1 hash)
void print_hash(int loop_index)
{
    for (int i = 0; i < loop_index; i++)
    {
        printf("#");
    }
}
