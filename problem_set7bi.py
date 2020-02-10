from sys import argv
import csv
import cs50


def main():

    # check for correct number of arguments
    if len(argv) != 2:
        print("Usage: import.py file.csv")
        return 1

    # open database
    db = cs50.SQL("sqlite:///students.db")

    # open csv file from argv
    with open(argv[1], "r") as csv_file:

        # create dictreader
        reader = csv.DictReader(csv_file)

        # loop through rows of csv file and add info to table in database
        for row in reader:
            names = extract_names(row["name"])

            if len(names) == 2:
                db.execute("INSERT INTO students (first, last, house, birth) VALUES(?, ?, ?, ?)",
                           names[0], names[1], row["house"], row["birth"])
            else:
                db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)",
                           names[0], names[1], names[2], row["house"], row["birth"])


# given fullname, extract individual names
def extract_names(fullname):

    # initialise variables
    names = []
    single_name = ''
    index = 0

    # loop through name string and add individual names to names array
    for c in fullname:
        if c == ' ':
            names.append(single_name)
            single_name = ''
            index += 1
        else:
            single_name += c

    # at the end of for loop, final name from fullname won't yet be added to names array
    names.append(single_name)

    return names


main()