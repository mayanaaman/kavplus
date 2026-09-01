from fib_module import fib


def test_fib_zero():
    assert fib(0) == 0


def test_fib_one():
    assert fib(1) == 1


def test_fib_two():
    assert fib(2) == 1


def test_fib_five():
    assert fib(5) == 5


def test_fib_ten():
    assert fib(10) == 55


def test_fib_twenty():
    assert fib(20) == 6765


def test_negative_input_current_behavior():
    assert fib(-1) == -1


def test_repeated_calls_are_consistent():
    assert fib(10) == 55
    assert fib(10) == 55
    assert fib(10) == 55
