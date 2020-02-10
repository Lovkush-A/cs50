from sys import argv
import cs50


def main():

    # check number of inputs in command line
    if len(argv) != 2:
        print("Usage: roster.py house")

    # open database file
    db = cs50.SQL("sqlite:///students.db")

    # execute sql command which selects rows with correct house
    students = db.execute("SELECT first, middle, last, birth FROM students WHERE house == '" + argv[1] + "' ORDER BY last, first")

    # loop through out of sql and print results
    for student in students:
        first = student['first']
        middle = student['middle']
        last = student['last']
        birth = student['birth']

        if middle == None:
            print(f"{first} {last}, born {birth}")
        else:
            print(f"{first} {middle} {last}, born {birth}")


main()
