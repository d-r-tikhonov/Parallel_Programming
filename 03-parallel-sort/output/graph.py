import pandas as pd
import matplotlib.pyplot as plt

data = pd.read_csv("03-parallel-sort/output/results.csv")
print(data)
plt.plot(data["Size"], data["Parallel Time"], label="Parallel qsort")
plt.plot(data["Size"], data["qsort Time"], label="qsort")
plt.xlabel("Array size")
plt.ylabel("Time (seconds)")
plt.title("Comparison of parallel sorting and qsort")
plt.legend()
plt.grid()
plt.savefig("03-parallel-sort/output/sort_comparison.png")