from attrs import define, field

INPUT = "./input/10"

connects: dict[tuple[int, int], str] = {
    (-1, 0): "SJL|",
    (1, 0): "SF7|",
    (0, -1): "S7J-",
    (0, 1): "SFL-",
}


@define
class Pipe:
    row: int = field()
    col: int = field()
    rows: list[str] = field()
    prev: tuple[int, int] = row, col
    char: str = "S"
    length: int = 0
    coords: list[tuple[int, int]] = []

    def get_interior_tiles(self):
        sum = 0
        for i in range(0, self.length):
            x_0, y_0 = self.coords[i]
            x_1, y_1 = self.coords[i - 1]
            sum += (x_0 * y_1) - (x_1 * y_0)
        return (sum - self.length) / 2 + 1

    def get_farthest_pipe(self):
        return self.length // 2 + self.length % 2


def look(pipe: Pipe, dir: tuple[int, int]) -> tuple[int, int]:
    row_next, col_next = pipe.row + dir[0], pipe.col + dir[1]
    can_move: bool = pipe.char in connects[dir]

    if can_move and pipe.prev != (row_next, col_next):
        pipe.prev = (pipe.row, pipe.col)
        pipe.coords.append(pipe.prev)
        pipe.char = pipe.rows[row_next][col_next]
        pipe.row = row_next
        pipe.col = col_next
        pipe.length += 1
        return pipe.row, pipe.col
    return -1, -1


def get_diagram(file: str) -> list[str]:
    with open(file, "r") as f:
        return [line.strip() for line in f.readlines()]


def find_start(diagram: list[str]) -> tuple[int, int]:
    row, col = 0, 0
    for line in diagram:
        col = line.find("S")
        if col >= 0:
            return row, col
        row += 1
    return -1, -1


def traverse(pipe: Pipe):
    pipe_type = ""
    while pipe_type != "S":
        look(pipe, (-1, 0))
        look(pipe, (1, 0))
        look(pipe, (0, -1))
        look(pipe, (0, 1))
        pipe_type = pipe.char


def main():
    diagram = get_diagram(INPUT)
    row, col = find_start(diagram)
    pipe = Pipe(row, col, diagram)
    traverse(pipe)

    print("\nAoC 2023 Day 10 - Part 1\n")
    print(f"Pipe length: {pipe.length}")
    print(f"Part 1: Farthest pipe is {pipe.get_farthest_pipe()} pipes away")
    print(f"Part 2: There are {pipe.get_interior_tiles()} interior tiles")


if __name__ == "__main__":
    main()
