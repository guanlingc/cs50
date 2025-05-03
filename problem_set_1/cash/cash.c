// coins avaliable 1c, 5c, 10c, 25c
// script should return the least number of coins for the change
// input is in cents

// import modules
#include <cs50.h>
#include <stdio.h>

// prototype
//  void change_to_coins(int change);

// main function
int main(void)
{
    // ask for change while change is less than 1 or not an int
    int change;
    do
    {
        change = get_int("Change owed: ");
    }
    while (change < 0);
    // numbers to check 25,10,5,1 using while loop
    // coin counter
    int coins = 0;
    while (change >= 1)
    {
        // change_to_coins(change);
        if (change >= 25)
        {
            change -= 25;
            coins++;
        }
        else if (change >= 10)
        {
            change -= 10;
            coins++;
        }
        else if (change >= 5)
        {
            change -= 5;
            coins++;
        }
        else
        {
            change -= 1;
            coins++;
        }
    }
    printf("%i", coins);
    printf("\n");
}

