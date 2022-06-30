from cs50 import get_int

# Ask user for height
height = 0
while height < 1 or height > 8:
    height = get_int("Height: ")
    
# Print pyramid
for i in range(1, height + 1):
    print(" " * (height - i), end="")
    print("#" * i)