import sys
import csv

# Exit if wrong user input
if len(sys.argv) != 3:
    print("Usage: python dna.py data.csv sequence.txt")
    sys.exit(1)

# Read people's information into memory
people = []
with open(sys.argv[1], "r") as database:
    people_reader = csv.DictReader(database)
    for row in people_reader:
        people.append(row)

# Read STRs into memory
strs = []
with open(sys.argv[1], "r") as database:
    reader = csv.reader(database)
    strs = next(reader)
    strs.remove("name")

# Read DNA sequence into memory
sequence = []
with open(sys.argv[2], "r") as file:
    sequence = file.read()

# Save max number of times each STR repeats in a dict
str_dict = {}
for s in strs:
    maximum = 0
    
    # Look for str "s" in the sequence
    j = len(s)
    for i in range(len(sequence)):
        if sequence[i: i + j] == s:
            
            # If str "s" was found, see number of times it repeats
            a = i
            count = 0
            while sequence[a: a + j] == s:
                count += 1
                a += j
            
            # Update maximum
            if count > maximum:
                maximum = count
    
    str_dict[s] = maximum

# Compare sequence given with database
for person in people:
    match = True
    for STR in str_dict:
        if str_dict[STR] != int(person[STR]):
            match = False
    
    if match:
        print(person["name"])
        sys.exit(0)

# No match
print("No match")
sys.exit(1)