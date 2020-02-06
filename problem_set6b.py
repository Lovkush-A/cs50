from cs50 import get_int


def main():

    # n is size of pyramids. keep asking until get one between 1 and 8
    n = get_int("Height: ")
    while n < 1 or n > 8:
        n = get_int("Height: ")

    # print pyramid one row at a time
    for i in range(n):
        print_row(n, i+1)


# this funtion prints ith row of n size pyramind
def print_row(n, i):
    print(" "*(n-i), end="")
    print("#"*i, end="")
    print("  ", end="")
    print("#"*i)


main()