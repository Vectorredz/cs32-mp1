'''
<< Mirror Flower. >>
Look into the Mirror...
'''
# --------------------------------------------------------- >>
# ----------------------------------------- <<
            #||-- LAYERS --||#
# << -----------------------------------------
'''
LAYER 0:
    >> INITIALIZATION TEST
        - MAKE (0 -> 2000)
        - MAKE (length of RANDOM_INTEGER(0, 2000))
    
LAYER 1:
    >> BASIC OPERATIONS TEST
        * Gazes at:
            - GET (Random Index)
            - SET (Random Index and Random Data)
            - PEEK_*
            - SIZE
            - EMPTY
            - REVERSE

LAYER 2:
    >> INSERTIONS/DELETIONS TEST
        * Gazes at:
            - PUSH_* (Random Data)
            - POP_*

LAYER 3:
    * Attempts to shatter the Reflection with:
        >> BASIC OPERATIONS TEST [Harder]
            - GET (Random Index)
            - SET (Random Index and Random Data)
            - PEEK_*
            - occasional REVERSE
            - SIZE, EMPTY
        >> INSERTIONS/DELETIONS TEST [Harder]
            - PUSH_* (Random Data)
            - POP_*
            - occasional REVERSE and SET (Random Index, Random Data)
            - SIZE, EMPTY

LAYER 4:
    * (No checking for correctness)
    * Attempts to shatter the Reflection one last time:
        >> TIME COMPLEXITY TEST:
            * if (LARGE_INPUTS == true):
                - PUSH_* (Random Data) for RANDOM_INTEGER(60000, 200000) times
                - POP_* for RANDOM_INTEGER(60000, 200000) times
                - Along with all other OPERATIONS throughout

        >> UB TEST:
            - Pops list until n == 0 first
            - All OPERATIONS for RANDOM_INTEGER(50000, 100000) times
            - Occassionally pops the list until 0 to test for UB

        >> FINALE:
            * if (LARGE_INPUTS == true):
                Pushes around RANDOM_INTEGER(60000, 200000) elements first (Random Data)
            - All OPERATIONS for RANDOM_INTEGER(50000, 100000) times
'''
from test_settings_m import INPUT_FILE, LARGE_INPUTS, SEED
# --------------------------------------------------------- >>
# --------------------------------------------------------- >>
# Do not edit past this point!
# --------------------------------------------------------- >>
# --------------------------------------------------------- >>
# --------------------------------------------------------- >>


from collections.abc import Sequence, Callable
from typing import TypeVar
import csv, random, os

random.seed(SEED)

# Make the Mirror class first (representing a working list)
DATA = TypeVar("DATA")
LENGTH = TypeVar("LENGTH")
type DATA = int
type LENGTH = int
class Mirror:
    _array: list[DATA]
    n: LENGTH
    def __init__(self, n: LENGTH, seq: Sequence[DATA]):
        self._array = list[DATA]()
        for i in range(n):
            self._array.append(seq[i])

        self.n = n
    
    def size(self) -> LENGTH:
        return self.n
    def empty(self) -> bool:
        return self.n == 0
    def reverse(self):
        self._array.reverse()

    def get(self, i: LENGTH) -> DATA:
        return self._array[i]
    def set(self, i: LENGTH, v: DATA):
        self._array[i] = v
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
    
    def TEST_elements(self): # dont modify using this lolo
        return self._array

# ----------------------------------------------------------------------
print("<< Mirror Flower. >>")
print("Look into the Mirror...")

print("> Initializing functions and file writing...")

# Helper functions for easier writing of test inputs
dataBound = 10**18
def randomData() -> DATA:
    return random.randint(-dataBound, dataBound)

def randomIndex(mirror: Mirror):
    if mirror.size() == 0:
        return 0
    return random.randint(0, mirror.size()-1)

def listToResult(seq: Sequence):
    if len(seq) == 0:
        return "EMPTY"
    return (",").join([str(data) for data in seq])

# ----------------------------------------------------------------------
def RESULT_size(mirror: Mirror):
    return str(mirror.size())
def RESULT_empty(mirror: Mirror):
    return "0" if mirror.empty() == False else "1"
def RESULT_reverse(mirror: Mirror):
    mirror.reverse()
    return listToResult(mirror.TEST_elements())

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
    return listToResult(mirror.TEST_elements())
def RESULT_push_right(mirror: Mirror, v: DATA):
    mirror.push_right(v)
    return listToResult(mirror.TEST_elements())
def RESULT_pop_left(mirror: Mirror):
    mirror.pop_left()
    return listToResult(mirror.TEST_elements())
def RESULT_pop_right(mirror: Mirror):
    mirror.pop_right()
    return listToResult(mirror.TEST_elements())

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

testFile = open(INPUT_FILE, "w+", newline="")
fields = ["OPERATION", "ARG1", "ARG2", "RESULT"]
writer = csv.DictWriter(testFile, fieldnames=fields, delimiter="|")
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

    # If checking for correctness then write RESULT as usual
    if checkForCorrectness == True:
        result = OPERATIONS[operation](mirror, *args)
    
    # If not then RESULT is "X"
    else:
        result = "X"
        getattr(mirror, operation)(*args)
    writer.writerow({
        "OPERATION": operation,
        "ARG1": args[0] if len(args) >= 1 else "None",
        "ARG2": str(args[1]) if len(args) >= 2 else "None",
        "RESULT": result
    })

# Custom messages within the test_input itself as the tester goes through each line (currently there are "LAYER"/"LAYERFIN")
def WRITECUSTOM(msg: int = "None", arg1: str = "None", arg2: str = "None", RESULT: str = "None"):
    writer.writerow({
        "OPERATION": msg,
        "ARG1": arg1,
        "ARG2": arg2,
        "RESULT": RESULT,
    })

print("> Done.")
# ------------------------------------------------------------------------------------------------
print("> Initializing tests...")


# ------------------------ LAYER 0: Initialize
print("> Layer 0...")
WRITECUSTOM("LAYER", str(0))

# test make (0 -> 2000)
for n in range(2000+1):
    seq = list[DATA]()
    for i in range(n):
        seq.append(randomData())
    mirror = Mirror(n, seq)
    writer.writerow({
        "OPERATION": "make",
        "ARG1": n,
        "ARG2": listToResult(seq),
        "RESULT": listToResult(mirror.TEST_elements())
    })

# test make (RANDOM_INTEGER(0, 2000))
n = random.randint(0, 2000)
seq = list[DATA]()
for i in range(n):
    seq.append(randomData())
mirror = Mirror(n, seq)
writer.writerow({
    "OPERATION": "make",
    "ARG1": n,
    "ARG2": listToResult(seq),
    "RESULT": listToResult(mirror.TEST_elements())
})

WRITECUSTOM("LAYERFIN", str(0))
print("> Done.")


# ------------------------ LAYER 1: No Insertions/Deletions
print("> Layer 1...")

WRITECUSTOM("LAYER", str(1))
# check initial indices
for i in range(mirror.size()):
    WRITE(True, mirror, "get", i)
for i in range(mirror.size()):
    WRITE(True, mirror, "set", i, randomData())

# check other operations
WRITE(True, mirror, "set", 0, randomData())
WRITE(True, mirror, "peek_left")
WRITE(True, mirror, "peek_right")

WRITE(True, mirror, "set", mirror.size()-1, randomData())
WRITE(True, mirror, "peek_left")
WRITE(True, mirror, "peek_right")

WRITE(True, mirror, "reverse")

WRITE(True, mirror, "get", 0)
WRITE(True, mirror, "get", mirror.size()-1)
WRITE(True, mirror, "set", 0, randomData())
WRITE(True, mirror, "get", 0)
WRITE(True, mirror, "get", mirror.size()-1)
WRITE(True, mirror, "peek_left")
WRITE(True, mirror, "peek_right")

WRITE(True, mirror, "get", 0)
WRITE(True, mirror, "get", mirror.size()-1)
WRITE(True, mirror, "set", mirror.size()-1, randomData())
WRITE(True, mirror, "get", 0)
WRITE(True, mirror, "get", mirror.size()-1)
WRITE(True, mirror, "peek_left")
WRITE(True, mirror, "peek_right")

WRITE(True, mirror, "reverse")

WRITE(True, mirror, "size")
WRITE(True, mirror, "empty")

WRITECUSTOM("LAYERFIN", str(1))
print("> Done.")


# ------------------------ LAYER 2: Consecutive
print("> Layer 2...")
WRITECUSTOM("LAYER", str(2))

# Consecutive Normal Pushes
for i in range(random.randint(500, 1000)):
    WRITE(True, mirror, "push_left", randomData())

# Consecutive Normal Pops
while (mirror.size() > 0):
    WRITE(True, mirror, "pop_left")
    

# Again.
for i in range(random.randint(500, 1000)):
    WRITE(True, mirror, "push_right", randomData())
while (mirror.size() > 0):
    WRITE(True, mirror, "pop_right")

# Double Consecutive Normal Punch
for i in range(random.randint(500, 1000)):
    operations = ["push_left", "push_right", "pop_left", "pop_right"]
    chosen = operations[random.randint(0, len(operations)-1)]
    if chosen == "pop_left" or chosen == "pop_right":
        WRITE(True, mirror, chosen)
    else:
        WRITE(True, mirror, chosen, randomData())
WRITECUSTOM("LAYERFIN", str(2))
print("> Done.")


# ------------------------ LAYER 3: Killer Move: Serious Series
print("> Layer 3...")
WRITECUSTOM("LAYER", str(3))

# Serious Punch
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

operations = ["push_left", "push_right", "pop_left", "pop_right"]
for i in range(random.randint(5000, 10000)):
    chosen = operations[random.randint(0, len(operations)-1)]
    if chosen.find("pop") != -1:
        WRITE(True, mirror, chosen)
    else:
        WRITE(True, mirror, chosen, randomData())
    if (random.randint(1, 10) == 1):
        WRITE(True, mirror, "reverse")
    if (random.randint(1, 5) == 1):
        WRITE(True, mirror, "set", randomIndex(mirror), randomData())
    WRITE(True, mirror, "size")
    WRITE(True, mirror, "empty")
WRITECUSTOM("LAYERFIN", str(3))
print("> Done.")


# ------------------------ LAYER 4: << dlroW derorriM >>
print("> Mirrored World...")
WRITECUSTOM("LAYER", "dlroW>><<derorriM")

for u in range(2):
    check = True
    lower, upper = 500, 1000
    if u == 1:
        if LARGE_INPUTS == False:
            break
        check = False
        lower = 60000
        upper = 200000

    for i in range(random.randint(lower, upper)):
        WRITE(check, mirror, "push_left", randomData())
        WRITE(check, mirror, "peek_left")
        WRITE(check, mirror, "peek_right")
        WRITE(check, mirror, "set", randomIndex(mirror), randomData())
        WRITE(check, mirror, "get", randomIndex(mirror))
        WRITE(check, mirror, "peek_left")
        WRITE(check, mirror, "peek_right")
        WRITE(check, mirror, "size")
        WRITE(check, mirror, "empty")
    while (mirror.size() > 0):
        WRITE(check, mirror, "pop_left")
        WRITE(check, mirror, "peek_left")
        WRITE(check, mirror, "peek_right")
        WRITE(check, mirror, "set", randomIndex(mirror), randomData())
        WRITE(check, mirror, "get", randomIndex(mirror))
        WRITE(check, mirror, "peek_left")
        WRITE(check, mirror, "peek_right")
        WRITE(check, mirror, "size")
        WRITE(check, mirror, "empty")

    for i in range(random.randint(lower, upper)):
        WRITE(check, mirror, "push_right", randomData())
        WRITE(check, mirror, "peek_left")
        WRITE(check, mirror, "peek_right")
        WRITE(check, mirror, "set", randomIndex(mirror), randomData())
        WRITE(check, mirror, "get", randomIndex(mirror))
        WRITE(check, mirror, "peek_left")
        WRITE(check, mirror, "peek_right")
        WRITE(check, mirror, "size")
        WRITE(check, mirror, "empty")
    while (mirror.size() > 0):
        WRITE(check, mirror, "pop_right")
        WRITE(check, mirror, "peek_left")
        WRITE(check, mirror, "peek_right")
        WRITE(check, mirror, "set", randomIndex(mirror), randomData())
        WRITE(check, mirror, "get", randomIndex(mirror))
        WRITE(check, mirror, "peek_left")
        WRITE(check, mirror, "peek_right")
        WRITE(check, mirror, "size")
        WRITE(check, mirror, "empty")

    while (mirror.size() > 0):
            WRITE(check, mirror, "pop_right")

for u in range(2):
    if u == 0:
        if random.randint(0, 8) == 0:
            while (mirror.size() > 0):
                WRITE(False, mirror, "pop_right")
    else:
        if LARGE_INPUTS == True:
            for i in range(random.randint(60000, 200000)):
                WRITE(False, mirror, "push_left", randomData())

    operations = list(OPERATIONS.keys())
    for i in range(random.randint(50000, 100000)):
        chosen = operations[random.randint(0, len(operations)-1)]
        args = ()
        if chosen == "get":
            args = tuple([randomIndex(mirror)])
        elif chosen == "set":
            args = tuple([randomIndex(mirror), randomData()])
        elif chosen == "push_left" or chosen == "push_right":
            args = tuple([randomData()])
        WRITE(False, mirror, chosen, *args)
            
WRITECUSTOM("LAYERFIN", "Mirrored<<>>World")
print("> Done.")

# Remove extra line at end of file
testFile.truncate(testFile.tell()-len(os.linesep))
print("Tests successfully initialized.")
print("Test operations can be found in (({INPUT_FILE:s}))\n".format(INPUT_FILE=INPUT_FILE))

print("Will you look into the Mirror once more?")