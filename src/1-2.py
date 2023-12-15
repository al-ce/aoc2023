import re

nums = ["one", "two", "three", "four", "five", "six", "seven", "eight", "nine"]
expr = re.compile(r"(?=(\d|%s))" % "|".join(nums))

sum = 0
with open("input", "r") as file:
    for line in file:
        found = [x in nums and nums.index(x) + 1 or int(x)
                 for x in expr.findall(line)]
        sum += found[0] * 10 + found[-1]

print(sum)
