import re
from functools import reduce


def get_fewest_cubes(line: str) -> list[int]:
    color_mins: dict[str, int] = {"red": 0, "green": 0, "blue": 0}
    cubes = re.findall(r"\d+ \w+", line)
    for draw in cubes:
        num, color = draw.split()
        if int(num) > color_mins[color]:
            color_mins[color] = int(num)
    return list(color_mins.values())


with open("input", "r") as f:
    sum = 0
    for line in f:
        fewest_cubes = get_fewest_cubes(line)
        sum += reduce(lambda x, y: x * y, fewest_cubes)

    print(sum)
