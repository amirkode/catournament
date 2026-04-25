#!/usr/bin/env python3

"""
Untuk menjalankan alat bantu testing, jalankan:

    python testing_tool.py [--verbose] <input file> <program> <arguments>

dengan `<arguments>` adalah argumen opsional untuk menjalankan program peserta.
Sebagai contoh:

    python testing_tool.py test.in ./myprogram
    python testing_tool.py test.in java -cp . Main
    python testing_tool.py test.in python myprogram.py

Jika argumen `--verbose` diberikan, maka alat bantu testing akan mengeluarkan
interaksi dengan program peserta.
"""

import argparse
import subprocess
import sys
from typing import List, TextIO


GRID_SIZE = 10
BATTLESHIP_LENGTH = 5
MAX_SHOTS = 30

WRONG_ANSWER = -1


class WrongAnswer(RuntimeError):
    """Raised whenever an incorrect answer is received."""

    pass


def vprint(*args, verbose: bool, file: TextIO, **kwargs) -> None:
    """Print to `file`, and also to stdout if `verbose` is true."""
    if verbose:
        print("< ", end="")
        print(*args, **kwargs)
        sys.stdout.flush()
    print(*args, file=file, flush=True, **kwargs)


def vreadline(data: TextIO, *, verbose: bool) -> str:
    """Read a line from `data`, and also log it to stdout if `verbose` is true."""
    line = data.readline()
    if verbose and line:
        print(">", line.rstrip("\n"))
    return line


def validate_input(grid: List[str]) -> None:
    def invalid_input(error_msg: str) -> None:
        raise ValueError(f"Input tidak valid: {error_msg}")
    
    if (len(grid) != GRID_SIZE or any([len(row) != GRID_SIZE for row in grid])):
        invalid_input("Ukuran grid tidak valid")

    if (any([any([c not in '01' for c in row]) for row in grid])):
        invalid_input("Terdapat karakter tidak valid")

    num_ones = 0
    for i in range(GRID_SIZE):
        for j in range(GRID_SIZE):
            num_ones += int(grid[i][j])
    if num_ones != BATTLESHIP_LENGTH:
        invalid_input("Banyaknya karakter '1'' tidak valid")

    def is_horizontal_ship(x: int, y: int) -> bool:
        for j in range(BATTLESHIP_LENGTH):
            if grid[x][y + j] != '1':
                return False
        return True

    def is_vertical_ship(x: int, y: int) -> bool:
        for i in range(BATTLESHIP_LENGTH):
            if grid[x + i][y] != '1':
                return False
        return True

    found_battleship = False
    for i in range(GRID_SIZE):
        for j in range(GRID_SIZE):
            if j + BATTLESHIP_LENGTH <= GRID_SIZE:
                found_battleship |= is_horizontal_ship(i, j)
            if i + BATTLESHIP_LENGTH <= GRID_SIZE:
                found_battleship |= is_vertical_ship(i, j)

    if not found_battleship:
        invalid_input("Battleship tidak ditemukan")


def interact(
    process: subprocess.Popen, grid: List[str], *, verbose: bool
) -> int:
    def parse_int(token: str, name: str) -> int:
        try:
            return int(token)
        except ValueError as exc:
            raise WrongAnswer(f"{name} bukan merupakan bilangan bulat") from exc

    shot_cells = set()
    shot_battleship_count = 0
    try:
        while True:
            line = vreadline(process.stdout, verbose=verbose)
            tokens = line.split()
            if len(tokens) != 2:
                vprint(WRONG_ANSWER, file=process.stdin, verbose=verbose)
                raise WrongAnswer(f"Invalid line: {line[:100]}")  # truncate

            x, y = parse_int(tokens[0], "x"), parse_int(tokens[1], "y")
            if not (1 <= x <= GRID_SIZE and 1 <= y <= GRID_SIZE):
                vprint(WRONG_ANSWER, file=process.stdin, verbose=verbose)
                raise WrongAnswer(f"Petak ({x}, {y}) tidak valid")
            
            if (x, y) in shot_cells:
                vprint(WRONG_ANSWER, file=process.stdin, verbose=verbose)
                raise WrongAnswer(f"Petak ({x}, {y}) sudah ditembak sebelumnya")
            shot_cells.add((x, y))

            shot_battleship_count += int(grid[x - 1][y - 1])
            if shot_battleship_count == BATTLESHIP_LENGTH:
                vprint(grid[x - 1][y - 1], file=process.stdin, verbose=verbose)
                break

            if len(shot_cells) == MAX_SHOTS:
                vprint(WRONG_ANSWER, file=process.stdin, verbose=verbose)
                raise WrongAnswer("Kapal musuh tidak tenggelam")

            vprint(grid[x - 1][y - 1], file=process.stdin, verbose=verbose)

    except BrokenPipeError as exc:
        raise WrongAnswer(
            "Tidak dapat mengirim masukan ke program peserta"
            " - mungkin sudah berhenti"
        ) from exc

    line = vreadline(process.stdout, verbose=verbose)
    if line.strip() != "":
        raise WrongAnswer("Terdapat keluaran berlebih dari program peserta")


def main() -> int:
    parser = argparse.ArgumentParser(
        usage="%(prog)s [--verbose] data.in program [args...]"
    )
    parser.add_argument(
        "--verbose", "-v", action="store_true", help="Keluarkan interaksi"
    )
    parser.add_argument("data")
    parser.add_argument("program", nargs=argparse.REMAINDER)

    args = parser.parse_args()
    if not args.program:
        parser.error("Harus sebutkan program untuk dijalankan")

    with open(args.data, "r", encoding="utf-8") as data:
        lines = data.readlines()
        grid = [row.strip() for row in lines]
        validate_input(grid)

    process = subprocess.Popen(
        args.program,
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        encoding="utf-8",
        errors="surrogateescape",
    )
    try:
        try:
            interact(process, grid, verbose=args.verbose)
        except WrongAnswer as exc:
            print("Wrong answer ({})".format(exc))
            return 1
        process.wait()
    finally:
        if process.poll() is None:
            try:
                process.terminate()
            except ProcessLookupError:
                pass
        process.wait()
    if process.returncode < 0:
        print(f"Run-time error (process exited with signal {-process.returncode})")
        return 1
    if process.returncode > 0:
        print(f"Run-time error (process exited with status {process.returncode})")
        return 1

    print(f"Accepted")
    return 0


if __name__ == "__main__":
    sys.exit(main())
