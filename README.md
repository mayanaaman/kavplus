# C/Python ctypes Binding Assignment

This project contains a simple Fibonacci binding demo and a Python binding for
the existing bus sorting project. The Python bindings use `ctypes` to call C
functions from shared libraries.

## Implemented Parts

- Fibonacci C binding demo.
- Bus project Python binding using `ctypes`.
- `pytest` tests for both the Fibonacci binding and bus binding.
- Code review and safety review notes for the C/Python boundary.

## Project Structure

- `fib_demo/fib.c`  
  Minimal C implementation of `int CFib(int n)` using regular recursion.

- `fib_demo/libfib.so`  
  Shared library compiled from `fib_demo/fib.c`.

- `fib_demo/fib_module.py`  
  Python `ctypes` module that loads `libfib.so`, configures `CFib` argument and
  return types, and exposes a clean `fib(n)` wrapper.

- `fib_demo/main.py`  
  Small demo script that imports `fib` from `fib_module.py` and prints
  `fib(10)`.

- `fib_demo/test_fib.py`  
  `pytest` tests for the Fibonacci Python binding.

- `bus_wrapper.c`  
  C wrapper layer for the bus sorting functions. It exposes Python-friendly
  functions that receive `BusLine *lines` and `int length`, then call the
  original pointer-range sorting functions.

- `libbus.so`  
  Shared library compiled from `bus_wrapper.c` and the original bus sorting C
  implementation.

- `bus_module.py`  
  Python `ctypes` module that loads `libbus.so`, mirrors the C `BusLine` struct,
  configures wrapper function signatures, and exposes Python sorting functions.

- `test_bus_module.py`  
  `pytest` tests for the bus Python binding.

## Compile Commands

Compile the Fibonacci shared library:

```bash
gcc -shared -fPIC -o fib_demo/libfib.so fib_demo/fib.c
```

Compile the bus shared library:

```bash
gcc -shared -fPIC -o libbus.so bus_wrapper.c sort_bus_lines.c
```

## Run the Fibonacci Demo

```bash
python3 fib_demo/main.py
```

Expected output:

```text
55
```

## Run Tests

```bash
python3 -m pytest fib_demo/test_fib.py test_bus_module.py
```

## Safety Notes

- The original C bus project files were not modified.
- `main.c` is not bound directly because it is a command-line program that reads
  from `stdin`, writes to `stdout`, parses arguments, and contains `main()`.
- Python loads local `.so` files using paths relative to the module files.
- `ctypes` executes native code directly, so shared libraries should be trusted.
- `make_bus_line` validates that names are `str` or `bytes` and that the encoded
  name length is shorter than `NAME_LEN`, leaving room for the C null terminator.
- Python integers are converted to C `int`, so C integer limits still apply.
- The Fibonacci implementation is intentionally simple recursion. Large inputs
  can be slow and may overflow the C `int` return value.

## Part A: Python Binding to C Code

Part A adds Python bindings to existing C code using `ctypes`. The bindings let
Python load compiled shared libraries and call C functions directly.

### Fibonacci Binding Demo

- `fib_demo/fib.c` contains the C implementation of the Fibonacci function.
- `fib_demo/libfib.so` is the compiled shared library built from `fib_demo/fib.c`.
- `fib_demo/fib_module.py` loads `libfib.so` with `ctypes`, configures the C
  function argument and return types, and exposes a Python-friendly wrapper.
- `fib_demo/test_fib.py` contains `pytest` tests for the Fibonacci binding.

### Bus Project Binding

- `bus_wrapper.c` provides Python-friendly wrapper functions around the original
  bus sorting functions.
- `libbus.so` is the compiled shared library built from `bus_wrapper.c` and the
  original bus sorting implementation.
- `bus_module.py` uses `ctypes` to load `libbus.so`, mirror the C `BusLine`
  struct, configure wrapper function signatures, and expose Python sorting
  helpers.
- `test_bus_module.py` contains `pytest` tests for the bus project binding.

The original bus C files were not modified for this binding layer.

## Part B: Memory and Pointer Visualization

Part B adds two standalone educational C visualization programs. These files are
for learning pointer behavior only. They do not modify or replace the original
sorting code.

### `memory_visualization.c`

- Visualizes bubble sort pointer behavior.
- Uses fake addresses for the array cells.
- Shows `start`, `end`, `i`, `j`, and `j + 1`.
- Shows comparisons, swaps, and the array state after each step.

Compile and run:

```bash
gcc -Wall -Wextra -Wvla -std=c99 memory_visualization.c -o memory_visualization
./memory_visualization
```

### `quick_sort_visualization.c`

- Visualizes quick sort partition pointer behavior.
- Uses fake addresses for the array cells.
- Shows `start`, `end`, `pivot`, `i`, and `j`.
- Shows comparison with the pivot, swaps, the final pivot swap, the returned
  pivot pointer, and the recursive ranges created after partition.

Compile and run:

```bash
gcc -Wall -Wextra -Wvla -std=c99 quick_sort_visualization.c -o quick_sort_visualization
./quick_sort_visualization
```

The fake addresses are used only for learning and are not real runtime memory
addresses.

## Part C: Interactive Bus Lab

Part C adds an interactive terminal frontend named `interactive_bus_lab.py`. It
combines the C-backed Python binding from Part A with the educational
visualization executables from Part B, making the project easier to explore and
present.

The interactive lab allows the user to:

- Load demo bus lines.
- Display the current bus lines in a table.
- Sort by name, distance, duration, and frequency using the C-backed Python
  binding.
- Run the bubble sort memory visualization.
- Run the quick sort memory visualization.
- Run all `pytest` tests.

Run the interactive lab:

```bash
python3 interactive_bus_lab.py
```

This is a safe frontend only:

- It does not modify the original C project files.
- It uses the existing `bus_module.py` binding.
- It runs the existing visualization executables.
- It is meant to make the project easier to explore and present.

### Optional Tkinter GUI

I also implemented `bus_sorting_gui.py`, a simple graphical interface that allows
the user to:

- Load demo bus lines.
- View them in a table.
- Sort by name, distance, duration, and frequency using the C-backed Python
  binding.
- Run the memory visualizations.
- Run all `pytest` tests.

Run the optional GUI:

```bash
python3 bus_sorting_gui.py
```

This GUI is optional and does not modify the original C files.

## AI-Agent Usage

An AI coding agent was used to help with architecture planning, C wrapper
generation, Python `ctypes` binding generation, pytest test generation, code
review, and safety review. Changes were reviewed before approval and were kept
separate from the original bus project C files.