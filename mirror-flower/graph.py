import matplotlib.pyplot as plt
import numpy as np
from enum import Enum
from pathlib import Path

# Size of inputs vs. Delta Time plot
class Implementation(Enum):
    DOUBLY_LINKED_LIST = 0
    DYNAMIC_ARRAY = 1
    SKIP_LIST = 2
    TREE_SEQUENCE = 3

def plot_data():
    total_size, total_deltaTime = [], []
    for operation in complexitiesPerOperation.keys():
        with Path("../mirror-flower/outputs/" + operation + ".txt").open("r") as file:

            size, deltaTime = [], []
            for line in file:
                parts = line.split("|")
                size.append(float(parts[0].strip()))
                deltaTime.append(float(parts[1].strip()))

            total_size.append(size)
            total_deltaTime.append(deltaTime)
            
    return total_size, total_deltaTime

def graph_settings(total_size, total_deltaTime):
    fig, axes1 =  plt.subplots(2,3)
    axes1[0, 0].plot(total_size[0],total_deltaTime[0],label=complexitiesPerOperation["make"][status.value])
    axes1[0, 0].set_title("Operation: make")
    axes1[0, 0].grid()
    axes1[0, 0].legend()

    axes1[0, 1].plot(total_size[1],total_deltaTime[1],label=complexitiesPerOperation["push_left"][status.value])
    axes1[0, 1].set_title("Operation: push_left")
    axes1[0, 1].grid()
    axes1[0, 1].legend()
    axes1[0, 1].set_xscale('log')
    axes1[0, 1].set_yscale('log')

    axes1[0, 2].plot(total_size[2],total_deltaTime[2],label=complexitiesPerOperation["push_right"][status.value])
    axes1[0, 2].set_title("Operation: push_right")
    axes1[0, 2].grid()
    axes1[0, 2].legend()

    axes1[1, 1].plot(total_size[3],total_deltaTime[3],label=complexitiesPerOperation["pop_left"][status.value])
    axes1[1, 1].set_title("Operation: pop_left")
    axes1[1, 1].grid()
    axes1[1, 1].legend()

    axes1[1, 2].plot(total_size[4],total_deltaTime[4],label=complexitiesPerOperation["pop_right"][status.value])
    axes1[1, 2].set_title("Operation: pop_right")
    axes1[1, 2].grid()
    axes1[1, 2].legend()

    axes1[1, 0].plot(total_size[5],total_deltaTime[5],label=complexitiesPerOperation["reverse"][status.value])
    axes1[1, 0].set_title("Operation: reverse")
    axes1[1, 0].grid()
    axes1[1, 0].legend()

    
    fig, axes2 =  plt.subplots(2, 3)
    axes2[0, 0].plot(total_size[6],total_deltaTime[6],label=complexitiesPerOperation["peek_left"][status.value])
    axes2[0, 0].set_title("Operation: peek_left")
    axes2[0, 0].grid()
    axes2[0, 0].legend()

    axes2[0, 1].plot(total_size[7],total_deltaTime[7],label=complexitiesPerOperation["peek_right"][status.value])
    axes2[0, 1].set_title("Operation: peek_right")
    axes2[0, 1].grid()
    axes2[0, 1].legend()

    axes2[0, 2].plot(total_size[8],total_deltaTime[8],label=complexitiesPerOperation["size"][status.value])
    axes2[0, 2].set_title("Operation: size")
    axes2[0, 2].grid()
    axes2[0, 2].legend()

    axes2[1, 0].plot(total_size[9],total_deltaTime[9],label=complexitiesPerOperation["get"][status.value])
    axes2[1, 0].set_title("Operation: get")
    axes2[1, 0].grid()
    axes2[1, 0].legend()

    axes2[1, 1].plot(total_size[10],total_deltaTime[10],label=complexitiesPerOperation["set"][status.value])
    axes2[1, 1].set_title("Operation: set")
    axes2[1, 1].grid()
    axes2[1, 1].legend()

    axes2[1, 2].plot(total_size[11],total_deltaTime[11],label=complexitiesPerOperation["empty"][status.value])
    axes2[1, 2].set_title("Operation: empty")
    axes2[1, 2].grid()
    axes2[1, 2].legend()
    return 



# Identify which implementation is being tested.
with Path("../mirror-flower/outputs/implementation.txt").open("r") as fileImple:
    currImple = [str(line.strip()) for line in fileImple][0] 

    complexitiesPerOperation ={
        "make" : ["O(l) worst-case", "O(l) worst-case", "O(l) expected", "O(l) worst-case"],
        "push_left" : ["O(1) worst-case", "O(1) amortized", "O(1) expected", "O(log(n)) worst-case"],
        "push_right" : ["O(1) worst-case", "O(1) amortized", "O(1) expected", "O(log(n)) worst-case"],
        "pop_left" : ["O(1) worst-case", "O(1) amortized", "O(1) expected", "O(log(n)) worst-case"],
        "pop_right" : ["O(1) worst-case", "O(1) amortized", "O(1) expected", "O(log(n)) worst-case"],
        "peek_left" : ["O(1) worst-case", "O(1) amortized", "O(1) expected", "O(1) worst-case"],
        "peek_right" : ["O(1) worst-case", "O(1) amortized", "O(log(n)) expected", "O(1) worst-case"],
        "size" : ["O(1) worst-case", "O(1) amortized", "O(log(n)) expected", "O(1) worst-case"],
        "empty" : ["O(1) worst-case", "O(1) amortized", "O(log(n)) expected", "O(1) worst-case"],
        "get" : ["O(1) worst-case", "O(1) amortized", "O(log(n)) expected", "O(log(n)) worst-case"],
        "set" : ["O(1) worst-case", "O(1) amortized", "O(log(n)) expected", "O(log(n)) worst-case"],
        "reverse" : ["O(1) worst-case", "O(1) amortized", "O(1) expected", "O(1) worst-case"]
        }
        

    # Set the status of the grapher
    match (currImple):
        case "DOUBLY_LINKED_LIST":
            status = Implementation.DOUBLY_LINKED_LIST
        case "DYNAMIC_ARRAY":
            status = Implementation.DYNAMIC_ARRAY
        case "SKIP_LIST":
            status = Implementation.SKIP_LIST
        case "TREE_SEQUENCE":
            status = Implementation.TREE_SEQUENCE

    total_size, total_deltaTime = plot_data()
    graph_settings(total_size, total_deltaTime)
    plt.show()
