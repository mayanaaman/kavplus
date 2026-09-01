import ctypes
from pathlib import Path


lib_path = Path(__file__).with_name("libfib.so")
lib = ctypes.CDLL(str(lib_path))

lib.CFib.argtypes = [ctypes.c_int]
lib.CFib.restype = ctypes.c_int


def fib(n):
    return lib.CFib(n)
