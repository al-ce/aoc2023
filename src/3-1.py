from string import digits


def is_symbol(char: str) -> int:
    return char not in digits and char != "."


def find_symbol(adj_line: str, start: int, cursor: int) -> bool:
    start = max(0, start - 1)
    end = min(cursor, len(adj_line)) + 1
    return any([is_symbol(x) for x in adj_line[start:end]])


def found_part_num(cursor: int, lines: list[str], row: int):
    num = ""
    start = cursor
    line = lines[row]
    while cursor < len(line) and line[cursor] in digits:
        num += line[cursor]
        cursor += 1

    # If we didn't build a number, there's nothing to check
    if not num:
        return 0

    prev = start > 0 and is_symbol(line[start - 1])
    next = cursor < len(line) - 1 and is_symbol(line[cursor])
    above = row > 0 and find_symbol(lines[row - 1], start, cursor)
    below = row < len(lines) - 1 and find_symbol(lines[row + 1], start, cursor)
    if any([prev, next, above, below]):
        return int(num)

    return 0


def get_part_nums_in_line(lines: list[str], row: int) -> int:
    line = lines[row]
    cursor = 0
    to_add = 0
    while cursor < len(line):
        num = found_part_num(cursor, lines, row)
        if num:
            cursor += len(str(num))
            to_add += num
        else:
            cursor += 1
    return to_add


def main():
    sum = 0
    with open("input/3", "r") as f:
        lines = [line.strip("\n") for line in f]

    for row in range(len(lines)):
        sum += get_part_nums_in_line(lines, row)

    print(sum)


if __name__ == "__main__":
    main()
