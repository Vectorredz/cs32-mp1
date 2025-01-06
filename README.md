# cs32-Implementations


## Contributors
Angchua, Jerwyn\
Paragas, Glenn\
Vilar, Louise

## Repository Contents

*\* The repository is divided into three sections:*\
**[List Implementations](#list-implementations)**\
**[Tester Files](#tester-files)**\
**[Miscellaneous](#miscellaneous)**

### List Implementations

*\* The directories for each of the 4 list implementations.\
Each contain their own **header** and **source** file, as well as a `test.c` file for manual testing. Example:* [d-linked-list.h](doubly-linked-list/d_linked_list.h) [d-linked-list.c](doubly-linked-list/d_linked_list.c) [test.c](doubly-linked-list/test.c)


**[doubly-linked-list](doubly-linked-list)** - Implementation of the Doubly Linked List.\
**[dynamic-arr](dynamic-arr)** - Implementation of Dynamic Array.\
**[skip-list](skip-list)** - Implementation of Skip List.\
**[tree-sequence](tree-sequence)** - Implementation of Sequence of Trees.

**[H_global.h](H_global.h)** - The global header file containing `LENGTH` (`size_t`) and `DATA` (`int64_t`), for easier conventions across all list implementations.

<br>

### Tester Files
*\* This directory contains all files relevant for testing.*\
*\* Please see **[Unit Test](#unit-test)** for more details.*

**[mirror-flower](mirror-flower)**

<br>

### Miscellaneous

**[PROOFS.md](PROOFS.md)** - Proof Sketches for the implementations.\
**[DETAILS.md](DETAILS.md)** - Implementation details for each of the lists.\
**[README.md](README.md)** - Me.

<br>

## Implementation of Code
For the list implementations, we used the references and concepts provided by the MP1 document for each of the implementations to implement our own version of each Abstract Data Type of a list.\
All lists are pointers of a `struct`, and any members for a `struct` are `pointers` to that `struct` as well, so that we only use the arrow `->` sugar operations for all lists for consistency.\
We also have the global types `LENGTH` (`size_t`) and `DATA` (`int64_t`) to represent values based on the length of the list and data values, respectively.\
Lastly, we aimed for $O(1)$ worst case for all lists' `reverse` operations.

***[DETAILS.md](DETAILS.md)** holds more details on each of the list's implementations.*


<br>

## Unit Test

For unit testing, we primarily used a controlled, randomized test generator for testing edge cases and determining efficiency.\
It is divided into the **Python-side**, and the **C-side**.

### Summary
*\* All relevant tester files are located in **[mirror-flower](mirror-flower)***\
[test_settings_m.py](mirror-flower/test_settings_m.py) - Generator (settings)\
[mirror.py](mirror-flower/mirror.py) - Generator

[test_settings_r.h](mirror-flower/test_settings_r.h) - Tester (settings)\
[reflection.c](mirror-flower/reflection.c) - Tester

[test_settings_r.h](mirror-flower/test_settings_r.h) - Grapher (settings)\
[graph.c](mirror-flower/graph.c) - Grapher Generator\
[graph.py](mirror-flower/graph.py) - Grapher Display

[inputs](mirror-flower/inputs) - Test inputs (operations)\
[outputs](mirror-flower/outputs) - Graph plot points\
[inputs/test_input_0.txt](mirror-flower/inputs/test_input_0.txt) - For testing the tester itself

<br>

### [PYTHON] Mirror (settings): *[test_settings_m.py](mirror-flower/test_settings_m.py)*
**[Python-side.]**\
The settings for the test generator.\
Imported as a Python module by the generator.
| SETTING | DATATYPE | DEFAULT |
| :------ | :------- | :------ |
| INPUT_DIRECTORY | `string` <br> The test inputs' file directory. | `inputs` |
| SEED | `any supported by random.seed` <br> The randomizer seed. | `None` |

<br>

### [PYTHON] Mirror: *[mirror.py](mirror-flower/mirror.py)*
**[Python-side.]**\
The generator for the tests. It acts as the "mirror" for the list to appropriately match as its reflection.\
It implements a working list in Python, and outputs test operations to the corresponding **INPUT_DIRECTORY**. These set of files are .txt files delimeted by a bar `|`, with its fields as follows:

> OPERATION|ARG1|ARG2|RESULT

Where **OPERATION** is the name of the operation function, **ARG1**/**ARG2** as the arguments for the function call, and **RESULT** being the correct resulting output that the target list needs to match to be considered correct.\
Their text can be of the following:

| OPERATION | ARG1 | ARG2 | RESULT (can be "**X**" to disable correctness) |
| :-------- | :--- | :--- | :----- |
| make | `LENGTH n` <br> number | `DATA *seq` <br> "**EMPTY**" <br> OR <br> number sequence, <br> separated by comma <br> (e.g. "**100,200,300**") | `raw list sequence` <br> "**EMPTY**" <br> OR <br> number sequence, <br> separated by comma <br> (e.g. "**100,200,300**") |
| size | "**None**" | "**None**" | `returned LENGTH` <br> number |
| empty | "**None**" | "**None**" | `returned bool` <br> "**0**" / "**1**" |
| reverse | "**None**" | "**None**" | `raw list sequence` <br> "**EMPTY**" <br> OR <br> number sequence, <br> separated by comma <br> (e.g. "**100,200,300**") |
| get | `LENGTH i` <br> number | "**None**" | `returned DATA` <br> number |
| set | `LENGTH i` <br> number | `DATA v` <br> number | `returned DATA` <br> number |
| peek_left | "**None**" | "**None**" | `returned DATA` <br> number |
| peek_right | "**None**" | "**None**" | `returned DATA` <br> number |
| push_left | `DATA v` <br> number | "**None**" | `raw list sequence` <br> "**EMPTY**" <br> OR <br> number sequence, <br> separated by comma <br> (e.g. "**100,200,300**") |
| push_right | `DATA v` <br> number | "**None**" | `raw list sequence` <br> "**EMPTY**" <br> OR <br> number sequence, <br> separated by comma <br> (e.g. "**100,200,300**") |
| pop_left | "**None**" | "**None**" | `raw list sequence` <br> "**EMPTY**" <br> OR <br> number sequence, <br> separated by comma <br> (e.g. "**100,200,300**") |
| pop_right | "**None**" | "**None**" | `raw list sequence` <br> "**EMPTY**" <br> OR <br> number sequence, <br> separated by comma <br> (e.g. "**100,200,300**") |

Note that an empty number sequence is represented by "**EMPTY**".\
Note that **make**, **reverse**, **push_\***, and **pop_\*** operations check for correctness on the entire list every time. This is to absolutely make sure that everything is working as expected within the actual list.\
Note also that **RESULT** can be set to "**X**" to disable checking for correctness at that line's execution. This is mainly for operations that are provided large inputs, and where checking for correctness is too expensive and takes too long.\
*\*Please see [inputs/test_input_0.txt](mirror-flower/inputs/test_input_0.txt) for a complete example.*

Lastly, by specifying **OPERATION** to be "**MSG**", the tester can send custom messages to the output stream using the other three columns, such as telling the user the current set of test types currently being performed, as it reads the .txt file line-by-line, or the beginning and end of a **Layer**.

<br>

<hr>

### Layers
For the actual tests themselves, to test for each ADT's correctness and efficiency, we have a **Layered Testing** system. The tests are divided into **Layers**.\
Each **Layer** tests a particular set of operations, edge cases, and concepts, with later layers potentially being harder to pass.\
This is so that it's easier to catch bugs in the earlier layers that test a set of operations instead of everything all at once, as the later layers throw eveything they can at the list to absolutely make sure nothing is broken and no lists pass if they have unnoticed broken edge cases, while being a bit more confusing to debug for the implementer.

> LAYER 0
```
INITIALIZATION TEST
    - MAKE (0 -> 1000)
    - MAKE (length of RANDOM_INTEGER(0, 1000))
    - with DATA in ranges [-10^18, 10^18]
```
This layer tests for the `make` operation, as well as the list's ability to hold DATA in large ranges.

<br>

>> LAYER 1
```
>> BASIC OPERATIONS TEST
    - GET (Random Index)
    - SET (Random Index and Random Data)
    - PEEK_*
    - SIZE
    - EMPTY
    - REVERSE
    * Repeat all of the above but done for each MAKE (0 -> 1000)
```
This layer is especially important for testing all non-insertion and non-deletion operations to see if they work as intended.\
Additionally, it repeats the basic operations test, but right after making a new list for each size $0$ to $1000$ again, to check for `make`'s correctness once more.


<br>

>>> LAYER 2:
```
>> INSERTIONS/DELETIONS TEST
    - PUSH_* (Random Data) (in sequence)
    - POP_* (in sequence)
```
This is the start of basic insertion/deletion operations. It does each `push` and `pop` operation in sequence (left first, then right.) Lists that don't implement it correctly have a high chance to fail here.

<br>

>>>> LAYER 3:
```
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
```
Attempts to break the list by doing random operations.\
It features more detailed basic operation tests, as well as more insertion/deletion operations. It may catch edge cases for some list implementations, and break them.

<br>

>>>>> LAYER 4:
```
>> BREAKER TEST:
    - PUSH_* (Random Data) for RANDOM_INTEGER(2000, 5000) times
    - POP_* until empty
    - Along with all other OPERATIONS throughout (to test for UB)

>> BREAKER TEST 2 (Random Operations)
    - PUSH_* (Random Data)
    - POP_*
    - Along with all other OPERATIONS throughout (to test for UB)
```
Attempts to shatter the Reflection, one last time, with testing all operations alongside a continuous insertion/deletion operation, for a large amount of times, to test for edge cases and UB. It throws everything it can towards the list. If a list was not caught broken before, it could be now.\
The most precise implemented lists with a couple of uncaught possible errors may have a difficult time passing this layer without catching any wrong edge cases.

<br>

>>>>>> LAYER 5:
```
> FINAL LAYER:
    >> FINALE:
        - PUSH_* (Random Data) for RANDOM_INTEGER(500, 1000) times first
        - All OPERATIONS for RANDOM_INTEGER(15000, 30000) times
        - bound size to <=4444
        - After the loop, pops list until n == 0
```
The final **Layer**. The tester gives up and surrenders itself to probability due to its inability to break the list in **Layer 4**. Now, it only tries to break the list with random operations. It is maybe possible that the list still breaks in this layer due to some unforeseen edge cases.

<br>

<hr>

### [C] Reflection (settings): *[test_settings_r.h](mirror-flower/test_settings_r.h)*
**[C-side].**\
The settings for the automatic tester.
Included as a C header by the tester.
| SETTING | VALUE | DEFAULT |
| :------ | :------- | :------ |
| IMPLEMENTATION | `DOUBLY_LINKED_LIST` / `DYNAMIC_ARRAY` / `SKIP_LIST` / `TREE_SEQUENCE` <br> The desired list to test. | `DOUBLY_LINKED_LIST` |
| LINE_DISPLAY | `boolean` <br> Whether to display the current line executing. <br> This is useful for segfaults <br> where the tester abruptly stops <br> and the faulty line is unknown. | `true` |
| CHECK_FOR_EFFICIENCY | `boolean` <br> Whether the automatic tester checks for efficiency (**TLE**). | `true` |
| TLE_BOUND | `double (milliseconds)` <br> Time boundary for throwing TLE. | `1000.0` |
| INPUT_DIRECTORY | `string` <br> The test inputs' file directory. | `inputs` |

<br>

*The tester directly includes a source file based on **IMPLEMENTATION**. I believe it may be an unorthodox way of testing, though it has helped us simplify things in the meantime.*

<br>

### [C] Reflection: *[reflection.c](mirror-flower/reflection.c)*
**[C-side.]**\
The automatic tester for all the generated test cases.\
It first obtains each line of the .txt and stores it in an array.\
Then, it sifts through each line. If the line's **RESULT** is not **X**, then it verifies for correctness and notifies the user if an operation's output failed to match **RESULT**. It also tests for its efficiency (if **CHECK_FOR_EFFICIENCY** setting is `true`.)

The tester also uses special test functions for its testing. These are global test operations that must be implemented for each list. They can be viewed in the **Global** section of **[DETAILS.md](DETAILS.md)**. In particular, it also verifies for the test operation `TEST_internal` to verify the internal tests.

If all tests pass, it notifies the user that they have passed all **Layers**.

<br>

<hr>

### [C] Grapher (settings): *[test_settings_g.h](mirror-flower/test_settings_g.h)*
The settings for the graph tester.
Included as a C header by the tester.
| SETTING | VALUE | DEFAULT |
| :------ | :------- | :------ |
| IMPLEMENTATION | `DOUBLY_LINKED_LIST` / `DYNAMIC_ARRAY` / `SKIP_LIST` / `TREE_SEQUENCE` <br> The desired list to test. | `DOUBLY_LINKED_LIST` |
| OUTPUT_DIRECTORY | `string` <br> The graph outputs' file directory. | `outputs` |

### [C] Grapher (Generator): *[graph.c](mirror-flower/graph.c)*
A specialized generator which tests the list against large inputs, without checking for correctness.\
After the tests, it outputs plot points/deltatime benchmarks for each operation in their own .txt file in **OUTPUT_DIRECTORY**.

### [PYTHON] Grapher (Display): *[graph.py](mirror-flower/graph.py)*
Displays the graphs using the outputs in **OUTPUT_DIRECTORY** for judging whether the graph of OPERATION is constant, linear, or logarithmic in nature.


<br>

<hr>

### Steps
With that, the steps are as follows:
1. **GENERATE NEW TEST CASES** *(Optional)*
    - Update [test_settings_m.py](mirror-flower/test_settings_m.py) settings to desired values
    - Run [mirror.py](mirror-flower/mirror.py)
2. **TEST THE LIST**
    - Update [test_settings_r.h](mirror-flower/test_settings_r.h) settings to desired values
    - Run [reflection.c](mirror-flower/reflection.c)
    - If any bugs are caught, debug with given error info and look at the line the operation failed in the files in **INPUT_DIRECTORY**
3. **GRAPHS** *(Optional)*
    - Update [test_settings_g.h](mirror-flower/test_settings_g.h) settings to desired values
    - Run [graph.c](mirror-flower/graph.c) to generate plot points
    - Run [graph.py](mirror-flower/graph.py) to display the graphs
    - Judge whether each operation is constant, linear, or logarithmic in nature. *(This is only for analyzing, and not a true determiner of its time complexity.)*


<br>


## Sources

### Doubly Linked List
- [Wikipedia: Skip list](https://en.wikipedia.org/wiki/Doubly_linked_list)

<br>

### Dynamic Array


<br>

### Skip List
- [Wikipedia: Skip list](https://en.wikipedia.org/wiki/Skip_list)
- [Skip Lists - Algorithms Lab](https://youtu.be/NDGpsfwAaqo?si=c4kY60b6UQ9rTAFk)
- [A Skip Lists Cookbook - William P.](https://webdiis.unizar.es/asignaturas/APD/skip_list_cookbook.pdf)

<br>

### Sequence of Trees
- [Wikipedia - Binomial heap](https://en.wikipedia.org/wiki/Binomial_heap)
- [NOI.PH DS 3 Section 6.2.2](https://drive.google.com/file/d/17zd_VzBMJ0tfHue1tGAh5Qo4jpXNPPQJ/view)
- [Data Structures in Typescript #17 - Binomial Heap Introduction by j4orz](https://www.youtube.com/watch?v=m8rsw3KfDKs&t=1394s&pp=ygUNYmlub21pYWwgaGVhcA%3D%3D)


### Tester
- [(stackoverflow.com) QueryPerformanceCounter - LowPart and HighPart](https://stackoverflow.com/questions/40584547/how-to-convert-an-integer-to-large-integer)


<hr>

<hr>
<hr>


<br>
<br>
<br>

砕けろ、鏡花水月。
<br>
<img src="https://drive.google.com/uc?export=view&id=1syDlPK6MJZfg5Y34IGWfgWkQ1NwiRBq3" width="854" height="auto" title="RORRIM______Our Reflection.">