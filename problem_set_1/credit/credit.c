// check validity of credit card number
// cc number are 16 or 13 and  16 digits
// AMEX starts with 3
// VISA starts with 4
// Master starts with 5
// of the 16 digits even positions *2
// odd positions
// sum of even*2 & odd
// if last digit is 0 then its valid

// imports
#include <cs50.h>
#include <stdbool.h>
#include <stdio.h>

// prototypes
int find_length(long cc_num);
void find_vendor(long cc_num, int length);
bool check_validity(long cc_num);

int main(void)
{
    long cc_num;
    do
    {
        // ask user to input cc number
        cc_num = get_long("Number? ");
    }
    while (cc_num < 0);
    // get length of cc number
    int length = find_length(cc_num);
    // check validity if true, find the vendor
    if (check_validity(cc_num))
    {
        find_vendor(cc_num, length);
    }
    else
    {
        printf("INVALID\n");
    }
}

// FUNCTIONS
int find_length(long cc_num)
{
    int length = 0;
    while (cc_num != 0)
    {
        cc_num /= 10;
        // printf("%li",cc_num);
        length++;
    }
    return length;
}

void find_vendor(long cc_num, int length)
{
    int first_2_digits;
    // keep dividing the cc until first digit
    while (cc_num >= 100)
    {
        cc_num /= 10;
    }
    first_2_digits = cc_num;

    if ((length == 15) && ((first_2_digits == 34) || (first_2_digits == 37)))
    {
        printf("AMEX\n");
    }
    else if (((length == 13) || (length == 16)) && cc_num / 10 == 4)
    {
        printf("VISA\n");
    }
    else if ((length == 16) && (first_2_digits >= 51) && (first_2_digits <= 55))
    {
        printf("MASTERCARD\n");
    }
    else
    {
        printf("INVALID\n");
    }
}

bool check_validity(long cc_num)
{
    // loop the odd and even positions
    // sum of every_other number, boolean to track
    int sum = 0;
    bool every_other_number = false;
    // when card still has numbers
    while (cc_num > 0)
    {
        // get last digit
        int last_digit = cc_num % 10;
        // process last digit (for *2)
        if (every_other_number)
        {
            last_digit *= 2;
            if (last_digit > 9)
            {
                last_digit -= 9;
            }
        }
        // add number to sum
        sum += last_digit;
        // remove last digit
        cc_num /= 10;
        // flip the alternate boolean
        every_other_number = !every_other_number;
    }
    // printf("%i", sum);
    return (sum % 10 == 0);
}
