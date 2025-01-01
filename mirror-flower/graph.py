import matplotlib.pyplot as plt
import numpy as np
# Size of inputs vs. DeltaTime plot
operations = ["make", "push_left", "push_right", "pop_left", "pop_right", "peek_left", "peek_right", "set", "get", "reverse", "empty", "size"]

total_size = []
total_deltaTime = []
def plot_data():
    for operation in operations:
        match operation:
            case "make":
                size = []
                deltaTime = []
                file = open("../mirror-flower/outputs/make.txt", "r")
                for line in file:
                    extract(line, size, deltaTime)
                total_deltaTime.append(deltaTime)
                total_size.append(size)    
            case "push_left":
                size = []
                deltaTime = []
                file = open("../mirror-flower/outputs/push_l.txt", "r")
                for line in file:
                    extract(line, size, deltaTime)
                total_deltaTime.append(deltaTime)
                total_size.append(size)    
            case "push_right":
                size = []
                deltaTime = []
                file = open("../mirror-flower/outputs/push_r.txt", "r")
                for line in file:
                    extract(line, size, deltaTime)
                total_deltaTime.append(deltaTime)
                total_size.append(size)    
            case "pop_left":
                size = []
                deltaTime = []
                file = open("../mirror-flower/outputs/pop_l.txt", "r")
                for line in file:
                    extract(line, size, deltaTime)
                total_deltaTime.append(deltaTime)
                total_size.append(size)    
            case "pop_right":
                size = []
                deltaTime = []
                file = open("../mirror-flower/outputs/pop_r.txt", "r")
                for line in file:
                    extract(line, size, deltaTime)
                total_deltaTime.append(deltaTime)
                total_size.append(size)    
            case "peek_left":
                size = []
                deltaTime = []
                file = open("../mirror-flower/outputs/peek_l.txt", "r")
                for line in file:
                    extract(line, size, deltaTime)
                total_deltaTime.append(deltaTime)
                total_size.append(size)    
            case "peek_right":
                size = []
                deltaTime = []
                file = open("../mirror-flower/outputs/peek_r.txt", "r")
                for line in file:
                    extract(line, size, deltaTime)
                total_deltaTime.append(deltaTime)
                total_size.append(size)    
            case "set":
                size = []
                deltaTime = []
                file = open("../mirror-flower/outputs/set.txt", "r")
                for line in file:
                    extract(line, size, deltaTime)
                total_deltaTime.append(deltaTime)
                total_size.append(size)    
            case "get":
                size = []
                deltaTime = []
                file = open("../mirror-flower/outputs/get.txt", "r")
                for line in file:
                    extract(line, size, deltaTime)
                total_deltaTime.append(deltaTime)
                total_size.append(size)         
            case "reverse":
                size = []
                deltaTime = []
                file = open("../mirror-flower/outputs/reverse.txt", "r")
                for line in file:
                    extract(line, size, deltaTime)
                total_deltaTime.append(deltaTime)
                total_size.append(size)       
            case "empty":
                size = []
                deltaTime = []
                file = open("../mirror-flower/outputs/empty.txt", "r")
                for line in file:
                    extract(line, size, deltaTime)  
                total_deltaTime.append(deltaTime)
                total_size.append(size)     
            case "size":
                size = []
                deltaTime = []
                file = open("../mirror-flower/outputs/size.txt", "r")
                for line in file:
                    extract(line, size, deltaTime) 
                total_deltaTime.append(deltaTime)
                total_size.append(size)  
    return


def extract(line, size, deltaTime):
    parts = line.split("|")
    size.append(float(parts[0].strip()))
    deltaTime.append(float(parts[1].strip()))

    return size, deltaTime

plot_data()

    
fig, axs =  plt.subplots(2,2)
axs[0, 0].plot(total_size[0],total_deltaTime[0])
axs[0, 0].set_title("Operation: make")
axs[0, 1].plot(total_size[1],total_deltaTime[1])
axs[0, 1].set_title("Operation: push_left")
axs[1, 0].plot(total_size[2],total_deltaTime[2])
axs[1, 0].set_title("Operation: push_right")
axs[1, 1].plot(total_size[3],total_deltaTime[3])
axs[1, 1].set_title("Operation: peek_left")
plt.show()




    
# deltaTime = [float(line.strip()) for line in deltaTime]

# # Plot the data
# plt.figure(figsize=(10, 6))
# plt.plot(deltaTime, size, marker='o', label="Time Complexity (seconds)")

# # Add labels, title, and legend
# plt.title("Size vs. DeltaTime", fontsize=16)
# plt.xlabel("Input Size", fontsize=12)
# plt.ylabel("DeltaTime", fontsize=12)
# # plt.grid(True, linestyle='--', alpha=0.6)
# plt.legend(fontsize=12)

# # Show the plot
# plt.show()