import subprocess
from pathlib import Path

from bus_module import (
    make_bus_line,
    sort_by_name,
    sort_by_distance,
    sort_by_duration,
    sort_by_frequency,
)


PROJECT_DIR = Path(__file__).resolve().parent


def load_demo_lines():
    return [
        make_bus_line("Dan", 40, 30, 5),
        make_bus_line("Alice", 10, 50, 3),
        make_bus_line("Charlie", 30, 20, 9),
        make_bus_line("Bob", 20, 40, 1),
    ]


def decode_name(line):
    return line.name.decode().rstrip("\x00")


def display_lines(lines):
    if not lines:
        print("No bus lines loaded yet. Choose option 1 first.")
        return

    print("+----+-----------+----------+----------+-----------+")
    print("| #  | Name      | Distance | Duration | Frequency |")
    print("+----+-----------+----------+----------+-----------+")
    for index, line in enumerate(lines):
        print(
            f"| {index:<2} | {decode_name(line):<9} | "
            f"{line.distance:<8} | {line.duration:<8} | {line.frequency:<9} |"
        )
    print("+----+-----------+----------+----------+-----------+")


def run_command(command):
    try:
        subprocess.run(command, cwd=PROJECT_DIR, check=False)
    except FileNotFoundError:
        print(f"Could not run: {' '.join(command)}")
        print("The command was not found on this system.")


def executable_path(name):
    plain_path = PROJECT_DIR / name
    exe_path = PROJECT_DIR / f"{name}.exe"

    if plain_path.exists():
        return f"./{name}"
    if exe_path.exists():
        return f"./{name}.exe"
    return None


def run_visualization(name, compile_command):
    executable = executable_path(name)
    if executable is None:
        print(f"Missing visualization executable: {name}")
        print("Compile it first with:")
        print(compile_command)
        return

    run_command([executable])


def run_pytest_tests():
    run_command([
        "python3",
        "-m",
        "pytest",
        "fib_demo/test_fib.py",
        "test_bus_module.py",
    ])


def print_menu():
    print("\nInteractive Bus Lab")
    print("1. Load built-in demo bus lines")
    print("2. Display current bus lines in a table")
    print("3. Sort by name")
    print("4. Sort by distance")
    print("5. Sort by duration")
    print("6. Sort by frequency")
    print("7. Run the bubble sort memory visualization executable")
    print("8. Run the quick sort memory visualization executable")
    print("9. Run all pytest tests")
    print("0. Exit")


def main():
    lines = []

    while True:
        print_menu()
        choice = input("Choose an option: ").strip()

        if choice == "1":
            lines = load_demo_lines()
            print("Loaded built-in demo bus lines.")
        elif choice == "2":
            display_lines(lines)
        elif choice == "3":
            lines = sort_by_name(lines)
            print("Sorted by name using the C bubble sort binding.")
            display_lines(lines)
        elif choice == "4":
            lines = sort_by_distance(lines)
            print("Sorted by distance using the C quick sort binding.")
            display_lines(lines)
        elif choice == "5":
            lines = sort_by_duration(lines)
            print("Sorted by duration using the C quick sort binding.")
            display_lines(lines)
        elif choice == "6":
            lines = sort_by_frequency(lines)
            print("Sorted by frequency using the C quick sort binding.")
            display_lines(lines)
        elif choice == "7":
            run_visualization(
                "memory_visualization",
                "gcc -Wall -Wextra -Wvla -std=c99 memory_visualization.c -o memory_visualization",
            )
        elif choice == "8":
            run_visualization(
                "quick_sort_visualization",
                "gcc -Wall -Wextra -Wvla -std=c99 quick_sort_visualization.c -o quick_sort_visualization",
            )
        elif choice == "9":
            run_pytest_tests()
        elif choice == "0":
            print("Goodbye.")
            break
        else:
            print("Invalid option. Please choose a number from the menu.")


if __name__ == "__main__":
    main()