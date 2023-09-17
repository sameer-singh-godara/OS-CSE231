import matplotlib.pyplot as plt

# Initialize empty lists to store data
indices = []
times = []
tempColors = ['red', 'blue', 'green']
intSched = ['OTHERS', 'FIFO', 'RR']
temp_index = [0, 1, 2]
#RED FOR OTHERS
# BLUE FOR FIFO
# GREEN FOR RR

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

categories = [intSched[i] for i in indices]
colors = [tempColors[i] for i in indices]


# Create a bar graph
bars = plt.bar(temp_index, times, align='center', alpha=0.7, color=colors)
plt.xlabel('Scheduling Process')
plt.xticks(temp_index, categories)
plt.ylabel('Time (seconds)')
plt.title('Execution Time for Each Process')

# Add text annotations on top of the bars
for bar, time in zip(bars, times):
    plt.text(bar.get_x() + bar.get_width() / 2, time + 0.05, str(time) + " seconds", ha='center', va='bottom')

# Display the graph
plt.savefig("graph.png")
plt.show()