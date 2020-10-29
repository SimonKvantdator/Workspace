import random

chars ="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()_+}{"
length = 13
password = ""

for i in range(length + 1):
    password += random.choice(chars)

print(password)

