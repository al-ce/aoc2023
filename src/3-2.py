from string import digits


def is_gear(char: str) -> bool:
    return char == "*"


def find_gear(adj_line: str, start: int, cursor: int) -> int:
    start = max(0, start - 1)
    end = min(cursor, len(adj_line)) + 1
    for i, char in enumerate(adj_line[start:end]):
        if is_gear(char):
            return start + i
    return -1


def find_above(lines: list[str], row: int, start: int, cursor: int) -> int:
    return row == 0 and -1 or find_gear(lines[row - 1], start, cursor)


def find_below(lines: list[str], row: int, start: int, cursor: int) -> int:
    max_row = len(lines) - 1
    return row == max_row and -1 or find_gear(lines[row + 1], start, cursor)


def check_left(line: str, start: int) -> bool:
    return start > 0 and is_gear(line[start - 1])


def check_right(line: str, cursor: int) -> bool:
    return cursor < len(line) - 1 and is_gear(line[cursor])


def find_gear_ratio(cursor: int, lines: list[str], row: int) -> tuple[int, int, int]:
    num_str = ""
    start = cursor
    line = lines[row]
    while cursor < len(line) and line[cursor] in digits:
        num_str += line[cursor]
        cursor += 1

    # If we didn't build a number, there's nothing to check
    if not num_str:
        return -1, -1, -1

    num = int(num_str)

    if check_left(line, start):
        return row, start - 1, num
    elif check_right(line, cursor):
        return row, cursor, num
    elif (gear_above := find_above(lines, row, start, cursor)) > -1:
        return row - 1, gear_above, num
    elif (gear_below := find_below(lines, row, start, cursor)) > -1:
        return row + 1, gear_below, num
    else:
        return -1, -1, -1


def update_gears_dict(gears: dict, coords: tuple[int, int], num: int):
    if not gears.get(coords):
        gears[coords] = [num]
    else:
        gears[coords].append(num)


def find_gears_in_line(lines: list[str], row: int, stars: dict):
    line = lines[row]
    cursor = 0
    while cursor < len(line):
        gear_row, gear_col, num = find_gear_ratio(cursor, lines, row)
        if num > -1:
            cursor += len(str(num))
            update_gears_dict(stars, (gear_row, gear_col), num)
        else:
            cursor += 1


def main():
    with open("./input/3", "r") as f:
        lines = [line.strip("\n") for line in f]

    gears: dict[tuple[int, int], list[int]] = {}
    for row in range(len(lines)):
        find_gears_in_line(lines, row, gears)

    sum_of_ratios = 0
    for nums in gears.values():
        if len(nums) == 2:
            sum_of_ratios += nums[0] * nums[1]

    print(sum_of_ratios)


if __name__ == "__main__":
    main()
