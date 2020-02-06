from cs50 import get_string


def main():

    # get input from user
    text = get_string("Text: ")

    # initialise variables. l w and s will store the number of letter words and sentences in text
    l = 0
    w = 0
    s = 0

    # loop through text, counting number of letters, words and sentences
    for c in text:
        if c.isalpha():
            l += 1
        elif c == " ":
            w += 1
        elif c in [".", "!", "?"]:
            s += 1

    # Final word in text has no space after it, so must be counted manually
    w += 1

    # calculate the coleman liau index
    L = l / w * 100
    S = s / w * 100
    index = round(0.0588 * L - 0.296 * S - 15.8)

    # print result
    if index < 1:
        print("Before Grade 1")
    elif index > 15:
        print("Grade 16+")
    else:
        print(f"Grade {index}")


main()