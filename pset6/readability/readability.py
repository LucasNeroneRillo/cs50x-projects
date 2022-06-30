from cs50 import get_string

# Prompt user for input
text = get_string("Text: ")

# Get number of letters, words, and sentences in text
letters, words, sentences = 0, 1, 0
for char in text:
    if char.isalpha():
        letters += 1
    elif char == " ":
        words += 1
    elif char in ["!", "?", "."]:
        sentences += 1

# Get L ans S per Coleman-Liau index
L = 100 * letters / words
S = 100 * sentences / words

# Calculate the index
index = round(0.0588 * L - 0.296 * S - 15.8)

# Print the text's grade
if index > 16:
    print("Grade 16+")
elif index < 1:
    print("Before Grade 1")
else:
    print(f"Grade {index}")