import ctypes
from pathlib import Path


NAME_LEN = 21


class BusLine(ctypes.Structure):
    _fields_ = [
        ("name", ctypes.c_char * NAME_LEN),
        ("distance", ctypes.c_int),
        ("duration", ctypes.c_int),
        ("frequency", ctypes.c_int),
    ]


lib_path = Path(__file__).with_name("libbus.so")
lib = ctypes.CDLL(str(lib_path))

BusLineArrayPointer = ctypes.POINTER(BusLine)

lib.sort_by_name_wrapper.argtypes = [BusLineArrayPointer, ctypes.c_int]
lib.sort_by_name_wrapper.restype = None

lib.sort_by_distance_wrapper.argtypes = [BusLineArrayPointer, ctypes.c_int]
lib.sort_by_distance_wrapper.restype = None

lib.sort_by_duration_wrapper.argtypes = [BusLineArrayPointer, ctypes.c_int]
lib.sort_by_duration_wrapper.restype = None

lib.sort_by_frequency_wrapper.argtypes = [BusLineArrayPointer, ctypes.c_int]
lib.sort_by_frequency_wrapper.restype = None


def make_bus_line(name, distance, duration, frequency):
    if isinstance(name, str):
        name = name.encode()
    elif not isinstance(name, bytes):
        raise TypeError("name must be str or bytes")
    if len(name) >= NAME_LEN:
        raise ValueError("name must be shorter than NAME_LEN bytes")
    return BusLine(name, distance, duration, frequency)


def _sort_lines(lines, sort_function):
    array_type = BusLine * len(lines)
    array = array_type(*lines)
    sort_function(array, len(lines))
    return list(array)


def sort_by_name(lines):
    return _sort_lines(lines, lib.sort_by_name_wrapper)


def sort_by_distance(lines):
    return _sort_lines(lines, lib.sort_by_distance_wrapper)


def sort_by_duration(lines):
    return _sort_lines(lines, lib.sort_by_duration_wrapper)


def sort_by_frequency(lines):
    return _sort_lines(lines, lib.sort_by_frequency_wrapper)
