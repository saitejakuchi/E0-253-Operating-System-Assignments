import os
import matplotlib.pyplot as plt


def addlabels(x, y):
    for i in range(1, len(x) + 1):
        plt.text(i, y[i - 1], y[i - 1], ha='center')


path_to_results = 'results'
file_name = 'plot_data'

fork_data = []
context_data = []
speedup_data = []
n_values = []
file_data = None


with open(os.path.join(path_to_results, file_name)) as file:
    file_data = file.readlines()

for lines in file_data:
    lines = lines.strip()
    if 'Index' in lines:
        n_values.append(int(lines.split('= ')[-1].strip()))
    elif 'fork' in lines:
        fork_data.append(int(lines.split(': ')[-1].strip()))
    elif 'context' in lines:
        context_data.append(int(lines.split(': ')[-1].strip()))
    else:
        speedup_data.append(float(lines.split(': ')[-1].strip()))

plt.figure(figsize=(18, 10))
plt.plot(n_values, fork_data, c='green')
plt.ylabel('Fork Throughput')
plt.xlabel('Number of elements (power of 2)')
plt.savefig(os.path.join(path_to_results, 'fork.png'), facecolor='white')
plt.close()

plt.figure(figsize=(18, 10))
plt.plot(n_values, context_data, c='blue')
plt.ylabel('Context Throughput')
plt.xlabel('Number of elements (power of 2)')
plt.savefig(os.path.join(path_to_results, 'context.png'), facecolor='white')
plt.close()


plt.figure(figsize=(18, 10))
plt.bar(n_values, speedup_data)
addlabels(n_values, speedup_data)
plt.ylabel('Speedup')
plt.xlabel('Number of elements (power of 2)')
plt.savefig(os.path.join(path_to_results, 'speedup.png'), facecolor='white')
plt.close()
