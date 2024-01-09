import re

LIMITS: dict[str, int] = {
    "red": 12,
    "green": 13,
    "blue": 14,
}


def is_possible(line: str) -> bool:
    cubes = re.findall(r"\d+ \w+", line)
    for draw in cubes:
        num, color = draw.split()
        if int(num) > LIMITS[color]:
            return False
    return True


with open("input", "r") as f:
    sum = 0
    for line in f:
        delimiter = line.index(":")
        game = int(line[5:delimiter])
        sum += game * is_possible(line)

    print(sum)
