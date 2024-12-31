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
        - MAKE (0 -> 1000)
        - MAKE (length of RANDOM_INTEGER(0, 1000))
    
LAYER 1:
    >> BASIC OPERATIONS TEST
        * Gazes at:
            - GET (Random Index)
            - SET (Random Index and Random Data)
            - PEEK_*
            - SIZE
            - EMPTY
            - REVERSE
        * Repeat all of the above but done for each MAKE (0 -> 1000)

LAYER 2:
    >> INSERTIONS/DELETIONS TEST
        * Gazes at:
            - PUSH_* (Random Data) (in sequence)
            - POP_* (in sequence)

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
    * Attempts to shatter the Reflection one last time:
        >> BREAKER TEST:
            - PUSH_* (Random Data) for RANDOM_INTEGER(2000, 5000) times
            - POP_* until empty
            - Along with all other OPERATIONS throughout

        >> EFFICIENCY TEST (if (LARGE_INPUTS == true)):
            > (No checking for correctness, "RESULT" is "X")
            - PUSH_* (Random Data) for RANDOM_INTEGER(60000, 200000) times
            - POP_* until empty
            - Along with all other OPERATIONS throughout

        >> UB TEST:
            - PUSH_* (Random Data) for RANDOM_INTEGER(500, 1000) times first
            - All OPERATIONS for RANDOM_INTEGER(5000, 10000) times
            - bound size to <=4444
            - Occassionally pops the list until 0 to test for UB
            - After the loop, pops list until n == 0

        >> EFFICIENCY TEST 2 (if (LARGE_INPUTS == true)):
            > (No checking for correctness, "RESULT" is "X")
                - Pops list until n == 0 first
                - Pushes around RANDOM_INTEGER(60000, 200000) elements first (Random Data)
                - All OPERATIONS for RANDOM_INTEGER(15000, 30000) times
                - After the loop, pops list until n == 0

        >> FINALE:
            - PUSH_* (Random Data) for RANDOM_INTEGER(500, 1000) times first
            - All OPERATIONS for RANDOM_INTEGER(15000, 30000) times
            - bound size to <=4444
            - After the loop, pops list until n == 0
'''
from test_settings_m import INPUT_DIRECTORY, LARGE_INPUTS, SEED
# --------------------------------------------------------- >>
# --------------------------------------------------------- >>
# Do not edit past this point!
# --------------------------------------------------------- >>
# --------------------------------------------------------- >>
# --------------------------------------------------------- >>


from collections.abc import Sequence, Callable
from typing import TypeVar
import csv, random, os
from csv import DictWriter
from pathlib import Path

random.seed(SEED)


# --------------------------------------------------------- >>
# ----------------------------------------- <<

            #||-- MIRROR LIST --||#

# ----------------------------------------- #
# --------------------------------------------------------- >>

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



# --------------------------------------------------------- >>
# ----------------------------------------- <<

        #||-- VARIABLES/HELPERS --||#

# ----------------------------------------- #
# --------------------------------------------------------- >>

print("<< Mirror Flower. >>")
print("Look into the Mirror...")

print("> Initializing functions and file writing...")

# Helper functions for easier writing of test inputs
dataBound = 10**4
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

def MAKEFILE(directory: Path):
    testFile = directory.open("w+", newline="")
    fields = ["OPERATION", "ARG1", "ARG2", "RESULT"]
    writer = csv.DictWriter(testFile, fieldnames=fields, delimiter="|")
    return testFile, writer

def WRITE(writer: DictWriter, checkForCorrectness: bool, mirror: Mirror, operation: str, *args):
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

print("> Done.")



# --------------------------------------------------------- >>
# ----------------------------------------- <<

        #||-- LAYERED TESTS --||#

# ----------------------------------------- #
# --------------------------------------------------------- >>

mirror: Mirror | None = None


# ------------------------ LAYER 0: Initialize
def LAYER0(writer: DictWriter):
    global mirror
    
    # test make (0 -> 1000)
    for n in range(1000+1):
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

    # test make (RANDOM_INTEGER(0, 1000))
    n = random.randint(0, 1000+1)
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


# ------------------------ LAYER 1: No Insertions/Deletions
print("> Layer 1...")

def LAYER1(writer: DictWriter):
    global mirror

    # check initial indices
    for i in range(mirror.size()):
        WRITE(writer, True, mirror, "get", i)
    for i in range(mirror.size()):
        WRITE(writer, True, mirror, "set", i, randomData())

    # check other operations
    def _basicCheck(mirror: Mirror):
        for _ in range(2):
            WRITE(writer, True, mirror, "get", 0)
            WRITE(writer, True, mirror, "get", mirror.size()-1)
            WRITE(writer, True, mirror, "peek_left")
            WRITE(writer, True, mirror, "peek_right")

            WRITE(writer, True, mirror, "reverse")

            WRITE(writer, True, mirror, "size")
            WRITE(writer, True, mirror, "empty")

        for _ in range(2):
            WRITE(writer, True, mirror, "get", 0)
            WRITE(writer, True, mirror, "get", mirror.size()-1)
            WRITE(writer, True, mirror, "peek_left")
            WRITE(writer, True, mirror, "peek_right")

            WRITE(writer, True, mirror, "set", 0, randomData())

            WRITE(writer, True, mirror, "get", 0)
            WRITE(writer, True, mirror, "get", mirror.size()-1)
            WRITE(writer, True, mirror, "peek_left")
            WRITE(writer, True, mirror, "peek_right")

            WRITE(writer, True, mirror, "set", mirror.size()-1, randomData())

            WRITE(writer, True, mirror, "get", 0)
            WRITE(writer, True, mirror, "get", mirror.size()-1)
            WRITE(writer, True, mirror, "peek_left")
            WRITE(writer, True, mirror, "peek_right")

            WRITE(writer, True, mirror, "reverse")

            WRITE(writer, True, mirror, "size")
            WRITE(writer, True, mirror, "empty")

    _basicCheck(mirror)

    # Combine with make (0 -> 1000)
    for n in range(1000+1):
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
        _basicCheck(mirror)

    # make again (RANDOM_INTEGER(0, 1000))
    n = random.randint(0, 1000+1)
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

print("> Done.")


# ------------------------ LAYER 2: Consecutive
print("> Layer 2...")

def LAYER2(writer: DictWriter):
    global mirror

    # Consecutive Normal Pushes
    for i in range(random.randint(500, 1000)):
        WRITE(writer, True, mirror, "push_left", randomData())

    # Consecutive Normal Pops
    while (mirror.size() > 0):
        WRITE(writer, True, mirror, "pop_left")
        

    # Again.
    for i in range(random.randint(500, 1000)):
        WRITE(writer, True, mirror, "push_right", randomData())
    while (mirror.size() > 0):
        WRITE(writer, True, mirror, "pop_right")

    # Double Consecutive Normal Punch
    for i in range(random.randint(500, 1000)):
        operations = ["push_left", "push_right", "pop_left", "pop_right"]
        chosen = operations[random.randint(0, len(operations)-1)]
        if chosen == "pop_left" or chosen == "pop_right":
            WRITE(writer, True, mirror, chosen)
        else:
            WRITE(writer, True, mirror, chosen, randomData())


    # ------------------------ LAYER 3: Killer Move: Serious Series

def LAYER3(writer: DictWriter):
    global mirror

    # Serious Punch
    for i in range(random.randint(5000, 10000)):
        r = random.randint(1, 4)
        if r == 1:
            WRITE(writer, True, mirror, "get", randomIndex(mirror))
        elif r == 2:
            WRITE(writer, True, mirror, "set", randomIndex(mirror), randomData())
        elif r == 3:
            WRITE(writer, True, mirror, "peek_left")
        elif r == 4:
            WRITE(writer, True, mirror, "peek_right")
            
        if random.randint(1, 10) == 1:
            WRITE(writer, True, mirror, "reverse")
        WRITE(writer, True, mirror, "size")
        WRITE(writer, True, mirror, "empty")

    operations = ["push_left", "push_right", "pop_left", "pop_right"]
    for i in range(random.randint(5000, 10000)):
        chosen = operations[random.randint(0, len(operations)-1)]
        if chosen.find("pop") != -1:
            WRITE(writer, True, mirror, chosen)
        else:
            WRITE(writer, True, mirror, chosen, randomData())
        if (random.randint(1, 10) == 1):
            WRITE(writer, True, mirror, "reverse")
        if (random.randint(1, 5) == 1):
            WRITE(writer, True, mirror, "set", randomIndex(mirror), randomData())
        WRITE(writer, True, mirror, "size")
        WRITE(writer, True, mirror, "empty")


# ------------------------ LAYER 4: << dlroW derorriM >>


def LAYER4(writer: DictWriter):
    global mirror

    # 1: Breaker Test, 2: Efficiency Test
    for u in range(1, 3):
        check = True
        lower, upper = 1000, 2222
        if u == 2:
            if LARGE_INPUTS == False:
                break
            check = False
            lower = 60000
            upper = 200000

        for i in range(random.randint(lower, upper)):
            WRITE(writer, check, mirror, "push_left", randomData())
            WRITE(writer, check, mirror, "peek_left")
            WRITE(writer, check, mirror, "peek_right")
            WRITE(writer, check, mirror, "set", randomIndex(mirror), randomData())
            WRITE(writer, check, mirror, "get", randomIndex(mirror))
            WRITE(writer, check, mirror, "peek_left")
            WRITE(writer, check, mirror, "peek_right")
            WRITE(writer, check, mirror, "size")
            WRITE(writer, check, mirror, "empty")
        while (mirror.size() > 0):
            WRITE(writer, check, mirror, "pop_left")
            WRITE(writer, check, mirror, "peek_left")
            WRITE(writer, check, mirror, "peek_right")
            WRITE(writer, check, mirror, "set", randomIndex(mirror), randomData())
            WRITE(writer, check, mirror, "get", randomIndex(mirror))
            WRITE(writer, check, mirror, "peek_left")
            WRITE(writer, check, mirror, "peek_right")
            WRITE(writer, check, mirror, "size")
            WRITE(writer, check, mirror, "empty")

        for i in range(random.randint(lower, upper)):
            WRITE(writer, check, mirror, "push_right", randomData())
            WRITE(writer, check, mirror, "peek_left")
            WRITE(writer, check, mirror, "peek_right")
            WRITE(writer, check, mirror, "set", randomIndex(mirror), randomData())
            WRITE(writer, check, mirror, "get", randomIndex(mirror))
            WRITE(writer, check, mirror, "peek_left")
            WRITE(writer, check, mirror, "peek_right")
            WRITE(writer, check, mirror, "size")
            WRITE(writer, check, mirror, "empty")
        while (mirror.size() > 0):
            WRITE(writer, check, mirror, "pop_right")
            WRITE(writer, check, mirror, "peek_left")
            WRITE(writer, check, mirror, "peek_right")
            WRITE(writer, check, mirror, "set", randomIndex(mirror), randomData())
            WRITE(writer, check, mirror, "get", randomIndex(mirror))
            WRITE(writer, check, mirror, "peek_left")
            WRITE(writer, check, mirror, "peek_right")
            WRITE(writer, check, mirror, "size")
            WRITE(writer, check, mirror, "empty")

def LAYER5(writer: DictWriter):
    # 1: Finale Test
    lower, upper = 10000, 15000
    for i in range(random.randint(500, 1000)):
        WRITE(writer, True, mirror, "push_left", randomData())

    operations = list(OPERATIONS.keys())
    for i in range(random.randint(lower, upper)):
        chosen = operations[random.randint(0, len(operations)-1)]
        args = ()
        if chosen == "get":
            args = tuple([randomIndex(mirror)])
        elif chosen == "set":
            args = tuple([randomIndex(mirror), randomData()])
        elif chosen == "push_left" or chosen == "push_right":
            args = tuple([randomData()])

        WRITE(writer, True, mirror, chosen, *args)

    # empty
    while (mirror.size() > 0):
        WRITE(writer, True, mirror, "pop_right")
            


print("> Initializing tests...")

if not INPUT_DIRECTORY.exists():
    INPUT_DIRECTORY.mkdir()
tests: dict[str, Callable[[DictWriter], None]] = {
    "0": LAYER0,
    "1": LAYER1,
    "2": LAYER2,
    "3": LAYER3,
    "4": LAYER4,
    "5": LAYER5,
}
for layer, func in tests.items():
    directory = INPUT_DIRECTORY / Path("LAYER" + layer + ".csv")

    if layer == "4":
        print("> Mirrored...")
    elif layer == "5":
        print("> World...")
    else:
        print("> Layer " + layer + "...")

    file, writer = MAKEFILE(directory)
    func(writer)

    # Remove extra line at end of file
    if file.tell() != 0:
        file.truncate(file.tell()-len(os.linesep))

    print("> Done.")


print("Tests successfully initialized.")
print("Test operations can be found in (({I:s}))\n".format(I=str(INPUT_DIRECTORY)))

print("Will you look into the Mirror once more?")