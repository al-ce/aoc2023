import re

total = 0
with open("input/4") as f:
    for line in f:
        nums = re.findall(r"\d+|\|", line)
        pipe = nums.index("|")
        wins, actual = nums[1:pipe], nums[pipe + 1:]
        found = 0
        for num in actual:
            found += (num in wins)
        if found:
            total += 2**(found - 1)

print(total)
