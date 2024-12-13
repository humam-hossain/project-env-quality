import serial
import matplotlib.pyplot as plt
from collections import deque

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

    # initialize data buffer
    data_buffer = deque([0] * MAX_POINTS, maxlen=MAX_POINTS)
    
    # setup the plot
    plt.ion()
    fig, ax = plt.subplots()
    line, = ax.plot(data_buffer, label="Data", color="#1f77b4")
    ax.set_ylim(-100, 100)
    ax.set_title("Serial Data Plotter")
    ax.set_xlabel("Time (ms)")
    ax.set_ylabel("Data")
    
    current_value_text = ax.text(0.02, 0.95, "", transform=ax.transAxes, fontsize=10, color="#ecf2f8", bbox=dict(facecolor="#21262d", edgecolor="#89929b", boxstyle="round"))
    
    # Add a legend
    ax.legend(loc="upper right", facecolor="#21262d", edgecolor="#89929b", labelcolor="#ecf2f8")
    
    print("[INFO] Plotting incoming data...")
    
    try:
        while plt.fignum_exists(fig.number):
            if ser.in_waiting > 0:
                line_data = ser.readline().decode("utf-8").strip()
                print(line_data)
                try:
                    value = int(line_data)
                    data_buffer.append(value)
                    line.set_ydata(data_buffer)
                    ax.set_xlim(0, len(data_buffer))
                    ax.set_ylim(min(data_buffer)-5, max(data_buffer)+5)
                    
                    # Update plot limits dynamically
                    ax.set_xlim(0, len(data_buffer))
                    ax.set_ylim(
                        (-max(data_buffer) * 1.5 if min(data_buffer) == 0 else -min(data_buffer) * 1.5), 
                        (max(data_buffer) * 1.5)
                    )
                    
                    # Update current value display
                    current_value_text.set_text(f"Value: {value}")
                    
                    ax.set_title(f"Serial Data Plotter (Last {len(data_buffer)} data)")
                    
                    plt.draw()
                    plt.pause(0.0001)
                except ValueError:
                    print(f"[ERROR] Failed to convert {line_data} to integer")
    except KeyboardInterrupt:
        print(f"[INFO:KeyboardInterrupt] Closing connection to {SERIAL_PORT}")
    finally:
        ser.close()
        plt.close(fig)
        print(f"[INFO] Connection to {SERIAL_PORT} closed")

if __name__ == "__main__":
    main()