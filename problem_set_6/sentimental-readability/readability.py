import cs50


def main():
    text = cs50.get_string("Text :")
    letters = cal_letters(text)
    words = cal_words(text)
    sentences = cal_sentences(text)
    index = cal_index(letters, words, sentences)

    if index >= 16:
        print("Grade 16+")
    elif index <= 1:
        print("Before Grade 1")
    else:
        print(f"Grade {index}")


def cal_letters(text):
    letters = 0
    for i in range(len(text)):
        if text[i].isalpha():
            letters += 1
    return letters


def cal_words(text):
    words = 1
    for i in range(len(text)):
        if text[i].isspace():
            words += 1
    return words


def cal_sentences(text):
    sentence = 0
    for i in range(len(text)):
        if text[i] in [".", "!", "?"]:
            sentence += 1

    return sentence


def cal_index(letters, words, sentences):
    L = (letters / words) * 100
    S = (sentences / words) * 100
    index = (0.0588 * L) - (0.296 * S) - 15.8

    return round(index)


if __name__ == "__main__":
    main()
