import random

print("Your password: ")

# list of all the possible characters
chars = "abcdefghijklmnopqrstuvwxyz1234567890!@#$%^&*()?"

# declare password variable as blank string
password = ''

# range of 16 is length of password
for x in range(16):
    # takes a random character from the list, add to password, and repeat 16 times
    password += random.choice(chars)

print (password)

