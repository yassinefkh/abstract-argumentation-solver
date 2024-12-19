import pandas as pd
import matplotlib.pyplot as plt

data = pd.read_csv("/Volumes/SSD/M1VMI/abstract-argumentation-solver/results.csv")

data["GraphSize"] = data["GraphSize"].astype(int)
data["NumAttacks"] = data["NumAttacks"].astype(int)
data["NaiveTime(ms)"] = data["NaiveTime(ms)"].astype(float)
data["CharFuncTime(ms)"] = data["CharFuncTime(ms)"].astype(float)

data["Density"] = data["NumAttacks"] / (data["GraphSize"] * (data["GraphSize"] - 1))

commands = data["Command"].unique()


def plot_time_comparison(data, commands):
    for command in commands:
        subset = data[data["Command"] == command]

        plt.figure(figsize=(10, 6))
        plt.plot(
            subset["GraphSize"],
            subset["NaiveTime(ms)"],
            label="Naive Approach (Time in ms)",
            marker="o",
        )
        plt.plot(
            subset["GraphSize"],
            subset["CharFuncTime(ms)"],
            label="Characteristic Function Approach (Time in ms)",
            marker="x",
        )

        plt.title(f"Execution Time Comparison: {command}")
        plt.xlabel("Graph Size (Number of Arguments)")
        plt.ylabel("Execution Time (ms)")
        plt.legend()
        plt.grid(True)

        plt.savefig(f"time_comparison_{command}.png")
        plt.show()


def plot_density_impact(data, commands):
    for command in commands:
        subset = data[data["Command"] == command]

        plt.figure(figsize=(10, 6))
        plt.scatter(
            subset["Density"],
            subset["NaiveTime(ms)"],
            label="Naive Approach (Time in ms)",
            alpha=0.7,
            marker="o",
        )
        plt.scatter(
            subset["Density"],
            subset["CharFuncTime(ms)"],
            label="Characteristic Function Approach (Time in ms)",
            alpha=0.7,
            marker="x",
        )

        plt.title(f"Impact of Attack Density on Execution Time ({command})")
        plt.xlabel("Attack Density")
        plt.ylabel("Execution Time (ms)")
        plt.legend()
        plt.grid(True)

        plt.savefig(f"density_impact_{command}.png")
        plt.show()


def plot_all_commands_time(data, commands):
    plt.figure(figsize=(12, 8))
    for command in commands:
        subset = data[data["Command"] == command]
        plt.plot(
            subset["GraphSize"],
            subset["NaiveTime(ms)"],
            label=f"Naive ({command})",
            marker="o",
        )
        plt.plot(
            subset["GraphSize"],
            subset["CharFuncTime(ms)"],
            label=f"CharFunc ({command})",
            marker="x",
        )

    plt.title("Execution Time Comparison Across Commands")
    plt.xlabel("Graph Size (Number of Arguments)")
    plt.ylabel("Execution Time (ms)")
    plt.legend()
    plt.grid(True)

    plt.savefig("all_commands_time_comparison.png")
    plt.show()


def generate_time_summary(data):
    summary = []

    for command in commands:
        subset = data[data["Command"] == command]
        naive_max_time = subset["NaiveTime(ms)"].max()
        charfunc_max_time = subset["CharFuncTime(ms)"].max()
        summary.append(
            {
                "Command": command,
                "Max Naive Time (ms)": naive_max_time,
                "Max CharFunc Time (ms)": charfunc_max_time,
                "Time Difference (Naive - CharFunc)": naive_max_time
                - charfunc_max_time,
            }
        )

    summary_df = pd.DataFrame(summary)
    summary_df.to_csv("time_summary_results.csv", index=False)
    print(summary_df)


plot_time_comparison(data, commands)
plot_density_impact(data, commands)
plot_all_commands_time(data, commands)
generate_time_summary(data)
