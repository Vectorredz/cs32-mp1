'''
Water Moon.
Will your Reflection be the same as mine?
'''


from collections.abc import Sequence, Callable
from typing import TypeVar
import csv, random

DATA = TypeVar("DATA")
LENGTH = TypeVar("LENGTH")
type DATA = int
type LENGTH = int

class Mirror:
    _array: list[DATA]
    n: LENGTH
    reversed: bool
    def __init__(self, n: LENGTH, seq: Sequence[DATA]):
        self._array = list[LENGTH]()
        for i in range(n):
            self._array.append(seq[i])

        self.n = n
        self.reversed = False
    
    def size(self) -> LENGTH:
        return self.n
    def empty(self) -> bool:
        return self.n == 0
    def reverse(self):
        self.reversed = not self.reversed

    def get(self, i: LENGTH) -> DATA:
        return self._array[i if not self.reversed else self.n-1-i]
    def set(self, i: LENGTH, v: DATA):
        self._array[i if not self.reversed else self.n-1-i] = v
    def peek_left(self) -> DATA:
        return self.get(0)
    def peek_right(self) -> DATA:
        return self.get(self.n-1)
    
    def push_left(self, v: DATA):
        self._array.insert(0, v)
        self.n += 1
    def push_right(self, v: DATA):
        self._array.append(v)
        self.n += 1
    def pop_left(self) -> bool:
        if self.n == 0: return False
        self._array.pop(0)
        self.n -= 1
        return True
    def pop_right(self) -> bool:
        if self.n == 0: return False
        self._array.pop()
        self.n -= 1
        return True
    
    def SPECIAL_getAllElements(self): # dont modify using this lolo
        return self._array


# ----------------------------------------------------------------------
def RESULT_size(mirror: Mirror):
    return str(mirror.size())
def RESULT_empty(mirror: Mirror):
    return str(mirror.empty())
def RESULT_reverse(mirror: Mirror):
    mirror.reverse()
    return str(mirror.SPECIAL_getAllElements())

def RESULT_get(mirror: Mirror, i: LENGTH):
    return str(mirror.get(i))
def RESULT_set(mirror: Mirror, i: LENGTH, v: DATA):
    mirror.set(i, v)
    return str(mirror.get(i))
def RESULT_peek_left(mirror: Mirror):
    return str(mirror.peek_left())
def RESULT_peek_right(mirror: Mirror):
    return str(mirror.peek_right())

def RESULT_push_left(mirror: Mirror, v: DATA):
    mirror.push_left(v)
    return str(mirror.SPECIAL_getAllElements())
def RESULT_push_right(mirror: Mirror, v: DATA):
    mirror.push_right(v)
    return str(mirror.SPECIAL_getAllElements())
def RESULT_pop_left(mirror: Mirror):
    mirror.pop_left()
    return str(mirror.SPECIAL_getAllElements())
def RESULT_pop_right(mirror: Mirror):
    mirror.pop_right()
    return str(mirror.SPECIAL_getAllElements())

OPERATIONS = dict()
OPERATIONS["size"] = RESULT_size
OPERATIONS["empty"] = RESULT_empty
OPERATIONS["reverse"] = RESULT_reverse
OPERATIONS["get"] = RESULT_get
OPERATIONS["set"] = RESULT_set
OPERATIONS["peek_left"] = RESULT_peek_left
OPERATIONS["peek_right"] = RESULT_peek_right
OPERATIONS["push_left"] = RESULT_push_left
OPERATIONS["push_right"] = RESULT_push_right
OPERATIONS["pop_left"] = RESULT_pop_left
OPERATIONS["pop_right"] = RESULT_pop_right

testFile = open("test_input.csv", "w", newline="")
fields = ["OPERATION", "ARG1", "ARG2", "RESULT"]
writer = csv.DictWriter(testFile, fieldnames=fields)
writer.writeheader

def WRITE(checkForCorrectness: bool, mirror: Mirror, operation: str, *args):
    # prevent UB
    if mirror.size() == 0:
        if operation == "get" \
            or operation == "set" \
            or operation == "peek_left" \
            or operation == "peek_right" \
            or operation == "pop_left" \
            or operation == "pop_right":
            return
    result = ""
    if checkForCorrectness == True:
        result = OPERATIONS[operation](mirror, *args)
    else:
        result = "X"
        getattr(mirror, operation)(*args)
    writer.writerow({
        "OPERATION": operation,
        "ARG1": args[0] if len(args) >= 1 else "None",
        "ARG2": str(args[1]) if len(args) >= 2 else "None",
        "RESULT": result
    })

testFile.truncate()

# ----------------------------------------------------------------------
dataBound = 10**18
def randomData() -> DATA:
    return random.randint(-dataBound, dataBound)

def randomIndex(mirror: Mirror):
    if mirror.size() == 0:
        return 0
    return random.randint(0, mirror.size()-1)



# ------------------------ LAYER 0: Initialize
_seq = list[DATA]()
for i in range(random.randint(0, 200)):
    _seq.append(randomData())
mirror = Mirror(len(_seq), _seq)
writer.writerow({
    "OPERATION": "make",
    "ARG1": "None",
    "ARG2": "None",
    "RESULT": str(mirror.SPECIAL_getAllElements())
})

# ------------------------ LAYER 1: No Insertions/Deletions

# check initial indices
for i in range(mirror.size()):
    WRITE(True, mirror, "get", i)

# random
for i in range(random.randint(5000, 10000)):
    r = random.randint(1, 4)
    if r == 1:
        WRITE(True, mirror, "get", randomIndex(mirror))
    elif r == 2:
        WRITE(True, mirror, "set", randomIndex(mirror), randomData())
    elif r == 3:
        WRITE(True, mirror, "peek_left")
    elif r == 4:
        WRITE(True, mirror, "peek_right")
        
    if random.randint(1, 10) == 1:
        WRITE(True, mirror, "reverse")
    WRITE(True, mirror, "size")
    WRITE(True, mirror, "empty")

# ------------------------ LAYER 2: Consecutive
# Consecutive Normal Pushes
for i in range(0, random.randint(500, 1000)):
    WRITE(True, mirror, "push_left", randomData())

# Consecutive Normal Pops
for i in range(0, random.randint(500, 1000)):
    WRITE(True, mirror, "pop_left")

# Again.
for i in range(0, random.randint(500, 1000)):
    WRITE(True, mirror, "push_right", randomData())
for i in range(0, random.randint(500, 1000)):
    WRITE(True, mirror, "pop_right")

# Double Consecutive Normal Punch
for i in range(0, random.randint(500, 1000)):
    operations = ["push_left", "push_right", "pop_left", "pop_right"]
    chosen = operations[random.randint(0, len(operations)-1)]
    if chosen == "pop_left" or chosen == "pop_right":
        WRITE(True, mirror, chosen)
    else:
        WRITE(True, mirror, chosen, randomData())


# ------------------------ LAYER 3: Killer Move: Serious Series

# Serious Punch
operations = ["push_left", "push_right", "pop_left", "pop_right"]
for i in range(0, random.randint(500, 1000)):
    chosen = operations[random.randint(0, len(operations)-1)]
    if chosen.find("pop") != -1:
        WRITE(True, mirror, chosen)
    else:
        WRITE(True, mirror, chosen, randomData())
    if (random.randint(1, 10) == 1):
        WRITE(True, mirror, "reverse")


# ------------------------ LAYER 4: [[ dlroW derorriM ]]

operations = list(OPERATIONS.keys())
for i in range(0, random.randint(100000, 200000)):
    chosen = operations[random.randint(0, len(operations)-1)]
    args = ()
    if chosen == "get":
        args = tuple([randomIndex(mirror)])
    elif chosen == "set":
        args = tuple([randomIndex(mirror), randomData()])
    elif chosen == "push_left" or chosen == "push_right":
        args = tuple([randomData()])

    WRITE(False, mirror, chosen, *args)






print("Will you look into the Mirror once more?")