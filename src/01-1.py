import re

sum = 0
with open("input", "r") as file:
    lines = file.readlines()
    for line in lines:
        line = line.strip("\n")
        digits = re.findall(r"\d", line)
        first, last = digits[0], digits[-1]
        sum += (int(first) * 10) + int(last)

print(sum)
