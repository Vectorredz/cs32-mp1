import matplotlib.pyplot as plt

filename = open("deldata.txt", "r")

def import_array_from_file(filename):
    array = [float(line.strip()) for line in filename]
    print(array)
    return array

array = import_array_from_file(filename)

x_values = range(1, len(array) + 1)

# Plot the data
plt.figure(figsize=(10, 6))
plt.plot(x_values, array, marker='o', label="Time Complexity (seconds)")

# Add labels, title, and legend
plt.title("Algorithm Time Complexity", fontsize=16)
plt.xlabel("Input Size / Index", fontsize=14)
plt.ylabel("Time (seconds)", fontsize=14)
plt.grid(True, linestyle='--', alpha=0.6)
plt.legend(fontsize=12)

# Show the plot
plt.show()