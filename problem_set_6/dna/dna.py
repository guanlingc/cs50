import csv
import sys
import os


def main():

    # TODO: Check for command-line usage
    # checks for 2 args, if print error and exit
    # print(sys.argv)
    if len(sys.argv) != 3:
        print("Usage .csv .txt")
        exit()
    # save arguements first csv, second txt
    csv_path = sys.argv[1]
    txt_path = sys.argv[2]
    # print(f"csv:path ={csv_path}, text_path = {txt_path}")
    # grab extension
    rows = []
    # TODO: Read database file into a variable
    csv_extension = os.path.splitext(csv_path)[1]
    if csv_extension == ".csv":
        with open(csv_path, "r") as csv_file:
            csv_contents = csv.DictReader(csv_file)
            short_seq = csv_contents.fieldnames[1:]
            for row in csv_contents:
                rows.append(row)

    # TODO: Read DNA sequence file into a variable
    txt_extension = os.path.splitext(txt_path)[1]
    if txt_extension == ".txt":
        with open(txt_path, "r") as txt_file:
            txt_contents = txt_file.read()

    # TODO: Find longest match of each STR in DNA sequence
    matching_dict = {}
    # for loop to compare each query with short_seq
    for i in range(len(short_seq)):
        longest_run = longest_match(txt_contents, short_seq[i])
        matching_dict.update({short_seq[i]: longest_run})

    # TODO: Check database for matching profiles
    # loop thru each dict of name and strs
    found_match = False
    for person in rows:
        current_preson_matches = 0
        for key in matching_dict.keys():
            if int(person[key]) == matching_dict[key]:
                current_preson_matches += 1
        if current_preson_matches == len(matching_dict):
            print(person['name'])
            found_match = True

    if found_match == False:
        print("No match")

    # print(row["name"])

    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


if __name__ == "__main__":
    main()
