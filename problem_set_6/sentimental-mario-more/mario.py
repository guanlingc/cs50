while True:
    # ask for height
    number = input("Height: ")
    # check for digits, reject strings
    if number.isdigit():
        number = int(number)
        # check range between 1 and 8
        if number > 0 and number < 9:
            break

# print block
for i in range(1, number+1):
    print(" " * (number-i), end="")
    print("#" * i, end="")
    print("  ", end="")
    print("#" * i)
