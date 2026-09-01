from bus_module import (
    make_bus_line,
    sort_by_name,
    sort_by_distance,
    sort_by_duration,
    sort_by_frequency,
)


def line_to_tuple(line):
    return (
        line.name.decode(),
        line.distance,
        line.duration,
        line.frequency,
    )


def sample_lines():
    return [
        make_bus_line("delta", 40, 30, 5),
        make_bus_line("alpha", 10, 50, 3),
        make_bus_line("charlie", 30, 20, 9),
        make_bus_line("bravo", 20, 40, 1),
    ]


def test_sort_by_distance_sorts_ascending():
    sorted_lines = sort_by_distance(sample_lines())
    assert [line.distance for line in sorted_lines] == [10, 20, 30, 40]


def test_sort_by_duration_sorts_ascending():
    sorted_lines = sort_by_duration(sample_lines())
    assert [line.duration for line in sorted_lines] == [20, 30, 40, 50]


def test_sort_by_frequency_sorts_ascending():
    sorted_lines = sort_by_frequency(sample_lines())
    assert [line.frequency for line in sorted_lines] == [1, 3, 5, 9]


def test_sort_by_name_sorts_alphabetically():
    sorted_lines = sort_by_name(sample_lines())
    assert [line.name.decode() for line in sorted_lines] == [
        "alpha",
        "bravo",
        "charlie",
        "delta",
    ]


def test_sorting_empty_list_returns_empty_list():
    assert sort_by_distance([]) == []


def test_sorting_single_item_list_returns_same_item():
    line = make_bus_line("solo", 7, 20, 4)
    sorted_lines = sort_by_name([line])
    assert len(sorted_lines) == 1
    assert line_to_tuple(sorted_lines[0]) == line_to_tuple(line)


def test_sorting_preserves_all_bus_line_records():
    lines = sample_lines()
    sorted_lines = sort_by_distance(lines)

    assert sorted(line_to_tuple(line) for line in sorted_lines) == sorted(
        line_to_tuple(line) for line in lines
    )


def test_too_long_name_raises_value_error():
    try:
        make_bus_line("a" * 21, 10, 20, 3)
    except ValueError:
        pass
    else:
        assert False, "expected ValueError"


def test_non_string_name_raises_type_error():
    try:
        make_bus_line(123, 10, 20, 3)
    except TypeError:
        pass
    else:
        assert False, "expected TypeError"


def test_sorting_does_not_mutate_original_python_list():
    lines = sample_lines()
    original_records = [line_to_tuple(line) for line in lines]

    sorted_lines = sort_by_name(lines)

    assert [line_to_tuple(line) for line in lines] == original_records
    assert [line.name.decode() for line in sorted_lines] == [
        "alpha",
        "bravo",
        "charlie",
        "delta",
    ]


def test_duplicate_sort_keys_do_not_lose_records():
    lines = [
        make_bus_line("alpha", 10, 40, 3),
        make_bus_line("bravo", 10, 20, 7),
        make_bus_line("charlie", 5, 30, 1),
    ]

    sorted_lines = sort_by_distance(lines)

    assert [line.distance for line in sorted_lines] == [5, 10, 10]
    assert sorted(line_to_tuple(line) for line in sorted_lines) == sorted(
        line_to_tuple(line) for line in lines
    )
