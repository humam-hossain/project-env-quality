import serial
import matplotlib.pyplot as plt
from collections import deque
import re

# plot config
plt.rcParams["axes.facecolor"] = "#21262d"
plt.rcParams["axes.edgecolor"] = "#89929b"
plt.rcParams["axes.labelcolor"] = "#ecf2f8"
plt.rcParams["axes.titlecolor"] = "#ecf2f8"
plt.rcParams["xtick.color"] = "#ecf2f8"
plt.rcParams["ytick.color"] = "#ecf2f8"
plt.rcParams["figure.facecolor"] = "#21262d"
plt.rcParams["legend.labelcolor"] = "#ecf2f8"

# config
SERIAL_PORT = "/dev/ttyUSB0"
BAUD_RATE = 9600
MAX_POINTS = 100

def main():
    try:
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
        print(f"[SUCCESS] Connected to {SERIAL_PORT} at {BAUD_RATE} baud")
        print("[INFO] Listening for incoming data...")
    except Exception as e:
        print(f"[ERROR] Failed to connect to {SERIAL_PORT} at {BAUD_RATE} baud.\n[ERROR] {e}")
        return

    # Initialize data buffers and lines for each key
    data_buffers = {}
    lines = {}
    
    # Setup the plot
    plt.ion()
    fig, ax = plt.subplots()
    ax.set_ylim(-100, 100)
    ax.set_title("Serial Data Plotter")
    ax.set_xlabel("Time (ms)")
    ax.set_ylabel("Data")
    ax.legend(loc="upper right", facecolor="#21262d", edgecolor="#89929b", labelcolor="#ecf2f8")
    
    # Display raw data in the top-left corner
    raw_data_text = ax.text(
        0.02, 0.98, "", 
        transform=ax.transAxes, fontsize=10, color="#ecf2f8", 
        verticalalignment='top'
    )
    
    print("[INFO] incoming data...")
    
    try:
        while plt.fignum_exists(fig.number):
            if ser.in_waiting > 0:
                line_data = ser.readline().decode("utf-8").strip()
                raw_data_text.set_text(line_data)  # Update displayed raw data
                
                pairs = re.findall(r"(\w+):(-?\d+\.\d+)", line_data)  # Supports negative values
                data_dict = {key: float(value) for key, value in pairs}
                print(data_dict)

                # Update buffers and create lines dynamically
                for key, value in data_dict.items():
                    if key not in data_buffers:
                        # Create a new buffer and line for the key
                        data_buffers[key] = deque([0] * MAX_POINTS, maxlen=MAX_POINTS)
                        lines[key], = ax.plot(data_buffers[key], label=key)
                        ax.legend()  # Update legend
                    # Append the new value to the buffer
                    data_buffers[key].append(value)
                    # Update the line data
                    lines[key].set_ydata(data_buffers[key])
                
                # Update plot limits dynamically
                all_values = [value for buffer in data_buffers.values() for value in buffer]
                if all_values:
                    ax.set_xlim(0, MAX_POINTS)
                    ax.set_ylim(min(all_values) - 5, max(all_values) + 5)
                
                plt.draw()
                plt.pause(0.0001)
    except KeyboardInterrupt:
        print(f"[INFO:KeyboardInterrupt] Closing connection to {SERIAL_PORT}")
    finally:
        ser.close()
        plt.close(fig)
        print(f"[INFO] Connection to {SERIAL_PORT} closed")

if __name__ == "__main__":
    main()