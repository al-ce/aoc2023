from attrs import define, field

INPUT = "./input/10"

connects: dict[tuple[int, int], str] = {
    (-1, 0): "SJL|",
    (1, 0): "SF7|",
    (0, -1): "S7J-",
    (0, 1): "SFL-",
}


@define
class Coord:
    row: int = field()
    col: int = field()

    def __add__(self, other) -> "Coord":
        return Coord(self.row + other.row, self.col + other.col)


@define
class Loop:
    curr: Coord = field()
    prev: Coord = field()
    rows: list[str] = field()
    char: str = "S"
    length: int = 0
    vertices: list[Coord] = []

    def get_interior_tiles(self) -> int:
        sum = 0
        for i in range(0, self.length):
            c_0 = self.vertices[i]
            c_1 = self.vertices[i - 1]
            sum += (c_1.col * c_0.row) - (c_0.col * c_1.row)

        interior = (sum - self.length) / 2 + 1
        return int(interior)

    def get_farthest_pipe(self) -> int:
        return self.length // 2 + self.length % 2


def look(loop: Loop, dir: Coord):
    next: Coord = loop.curr + dir
    can_move: bool = loop.char in connects[(dir.row, dir.col)]

    if can_move and loop.prev != next:
        loop.prev = loop.curr
        loop.curr = next
        loop.vertices.append(loop.curr)
        loop.char = loop.rows[next.row][next.col]
        loop.length += 1


def get_diagram(file: str) -> list[str]:
    with open(file, "r") as f:
        return [line.strip() for line in f.readlines()]


def find_start(diagram: list[str]) -> Coord:
    row, col = 0, 0
    for line in diagram:
        col = line.find("S")
        if col >= 0:
            return Coord(row, col)
        row += 1
    return Coord(-1, -1)


def traverse(loop: Loop):
    pipe_type = ""
    connect_keys = connects.keys()
    while pipe_type != "S":
        for x, y in connect_keys:
            look(loop, Coord(x, y))
        pipe_type = loop.char


def main():
    diagram = get_diagram(INPUT)
    start = find_start(diagram)
    loop = Loop(start, start, diagram)
    loop.vertices.append(loop.curr)
    traverse(loop)

    print("\nAoC 2023 Day 10\n")
    print(f"Loop starts at: {start.row}, {start.col}")
    print(f"Loop length: {loop.length}")
    print(f"Part 1: Farthest pipe is {loop.get_farthest_pipe()} pipes away")
    print(f"Part 2: There are {loop.get_interior_tiles()} interior tiles")


if __name__ == "__main__":
    main()
