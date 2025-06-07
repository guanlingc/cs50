# outut should be AMEX\n

import cs50


def main():
    while True:
        # ask for number
        card_number = cs50.get_string("Number: ")

        card_len = len(card_number)
        card_number = int(card_number)
        if check_validity(card_number):
            find_vendor(card_number, card_len)
            break
        else:
            print("INVALID")
            break


def check_validity(card_number):
    # set variables
    sum = 0
    every_other_number = False
    while card_number > 0:
        # get last digit
        last_digit = card_number % 10
        # every 2nd digit needs to multiply by 2
        if every_other_number:
            last_digit *= 2
            if last_digit > 9:
                last_digit -= 9
        # update_sum
        sum += last_digit
        # remove last_digit
        card_number //= 10
        # flip card number condition
        every_other_number = not every_other_number

    return (sum % 10 == 0)


def find_vendor(card_number, length):
    # first_2_digits = 0
    while card_number >= 100:
        card_number //= 10
    first_2_digits = card_number

    if length == 15 and first_2_digits in [34, 37]:
        print("AMEX")
    elif length in [13, 16] and (first_2_digits // 10 == 4):
        print("VISA")
    elif length == 16 and first_2_digits in [51, 55]:
        print("MASTERCARD")
    else:
        print("INVALID")


if __name__ == "__main__":
    main()
