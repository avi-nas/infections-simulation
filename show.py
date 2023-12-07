import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.widgets import Button

# Replace the file path with your actual CSV file path
file_path = r"simulation_output.csv"

class TimestepPlotter:
    def __init__(self, data):
        self.data = data
        self.timesteps = data['TimeStep'].unique()
        self.current_timestep_index = 0

        self.fig, self.ax = plt.subplots(figsize=(10, 6))
        self.plot_current_timestep()
        self.add_navigation_buttons()

    def plot_current_timestep(self):
        timestep_data = self.data[self.data['TimeStep'] == self.timesteps[self.current_timestep_index]]

        self.ax.clear()
        self.ax.bar(['Susceptible', 'Infected', 'Recovered', 'Vaccinated'], timestep_data.iloc[0, 1:], alpha=0.7)

        self.ax.set_ylabel('Count')
        self.ax.set_title(f'Bar Graphs for Day {self.timesteps[self.current_timestep_index]}')
        self.ax.legend()

        plt.draw()

    def on_prev_button_click(self, event):
        self.current_timestep_index = (self.current_timestep_index - 1) % len(self.timesteps)
        self.plot_current_timestep()

    def on_next_button_click(self, event):
        self.current_timestep_index = (self.current_timestep_index + 1) % len(self.timesteps)
        self.plot_current_timestep()

    def add_navigation_buttons(self):
        ax_prev = plt.axes([0.81, 0.01, 0.1, 0.075])
        ax_next = plt.axes([0.92, 0.01, 0.1, 0.075])

        prev_button = Button(ax_prev, 'Previous')
        next_button = Button(ax_next, 'Next')

        prev_button.on_clicked(self.on_prev_button_click)
        next_button.on_clicked(self.on_next_button_click)

        plt.show()
    
if __name__ == "__main__":
    # Read the CSV file into a DataFrame
    df = pd.read_csv(file_path)

    # Create an instance of TimestepPlotter
    plotter = TimestepPlotter(df)