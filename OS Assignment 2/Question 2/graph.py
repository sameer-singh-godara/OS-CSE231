import matplotlib.pyplot as plt

# Initialize empty lists to store data
indices = []
times = []

# Read data from the input file
with open('output.txt', 'r') as file:
    for line in file:
        parts = line.strip().split()
        if len(parts) == 2:
            index, time = int(parts[0]), float(parts[1])
            indices.append(index)
            times.append(time)

# Set the scale and limit for the y-axis
plt.ylim(0, 4)
plt.yticks([i * 0.1 for i in range(41)])

# Create a bar graph
plt.bar(indices, times, align='center', alpha=0.7)
plt.xlabel('Index')
plt.ylabel('Time (seconds)')
plt.title('Execution Time for Each Process')

# Display the graph
plt.show()
