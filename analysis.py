import os
import pandas as pd
import matplotlib.pyplot as plt
from datetime import datetime
from matplotlib.backends.backend_pdf import PdfPages

# Load data from CSV
filename = "data/serial_data_2024-12-17_21-38-07.csv"
df = pd.read_csv(filename)

# Parse the timestamp column to datetime
df['timestamp'] = pd.to_datetime(df['timestamp'], format='%Y-%m-%d %H:%M:%S')

# Calculate statistics
stats = {}
for col in df.columns:
    if df[col].dtype in ['float64', 'int64']:  # Only calculate for numerical columns
        col_mean = df[col].mean()
        col_median = df[col].median()
        col_mode = df[col].mode().iloc[0] if not df[col].mode().empty else None
        col_std = df[col].std()
        col_cv = (col_std / col_mean * 100) if col_mean != 0 else 0
        stats[col] = {
            'Mean': col_mean,
            'Median': col_median,
            'Mode': col_mode,
            'Std Dev': col_std,
            'CV%': col_cv
        }

# Convert stats to a DataFrame for better formatting
stats_df = pd.DataFrame(stats).T
stats_df.reset_index(inplace=True)
stats_df.columns = ['Column', 'Mean', 'Median', 'Mode', 'Std Dev', 'CV%']

# Calculate total duration
start_time = df['timestamp'].iloc[0]
end_time = df['timestamp'].iloc[-1]
duration = end_time - start_time

# Format the duration into hours, minutes, and seconds
duration_str = str(duration)

# Generate Plots
fig, axs = plt.subplots(3, 2, figsize=(12, 16))

# Subplot 1: pcs_1um and pcs_2.5um vs Time
axs[0, 0].plot(df['timestamp'], df['pcs_1um'], marker='o', color='b', label='pcs_1um')
axs[0, 0].plot(df['timestamp'], df['pcs_2.5um'], marker='x', color='m', label='pcs_2.5um')
axs[0, 0].set_title(f'Particle Counts vs Time (Duration: {duration_str})')
axs[0, 0].set_xlabel('Time (MM:SS)')
axs[0, 0].set_ylabel('Particle Count (pcs)')
axs[0, 0].legend()
axs[0, 0].grid(True)
axs[0, 0].tick_params(axis='x', rotation=45)

# Subplot 2: mgm3_1um and mgm3_2.5um vs Time
axs[0, 1].plot(df['timestamp'], df['mgm3_1um'], marker='o', color='g', label='mgm3_1um')
axs[0, 1].plot(df['timestamp'], df['mgm3_2.5um'], marker='x', color='orange', label='mgm3_2.5um')
axs[0, 1].set_title(f'Mass Concentration vs Time (Duration: {duration_str})')
axs[0, 1].set_xlabel('Time (MM:SS)')
axs[0, 1].set_ylabel('Mass Concentration (mg/m³)')
axs[0, 1].legend()
axs[0, 1].grid(True)
axs[0, 1].tick_params(axis='x', rotation=45)

# Subplot 3: r_1um and r_2.5um (Low Pulse Ratio) vs Time
axs[1, 0].plot(df['timestamp'], df['r_1um'], marker='o', color='purple', label='r_1um')
axs[1, 0].plot(df['timestamp'], df['r_2.5um'], marker='x', color='brown', label='r_2.5um')
axs[1, 0].set_title(f'Low Pulse Ratio vs Time (Duration: {duration_str})')
axs[1, 0].set_xlabel('Time (MM:SS)')
axs[1, 0].set_ylabel('Low Pulse Ratio (r)')
axs[1, 0].legend()
axs[1, 0].grid(True)
axs[1, 0].tick_params(axis='x', rotation=45)

# Subplot 4: samples vs Time
axs[1, 1].plot(df['timestamp'], df['samples'], marker='o', color='red', label='samples')
axs[1, 1].set_title(f'Samples vs Time (Duration: {duration_str})')
axs[1, 1].set_xlabel('Time (MM:SS)')
axs[1, 1].set_ylabel('Samples')
axs[1, 1].legend()
axs[1, 1].grid(True)
axs[1, 1].tick_params(axis='x', rotation=45)

# Subplot 5: t vs Time (time in seconds)
axs[2, 0].plot(df['timestamp'], df['t'], marker='o', color='blue', label='t (time in seconds)')
axs[2, 0].set_title(f'Time (t) vs Time\nTotal Duration: {duration_str}')
axs[2, 0].set_xlabel('Time (MM:SS)')
axs[2, 0].set_ylabel('Time (s)')
axs[2, 0].legend()
axs[2, 0].grid(True)
axs[2, 0].tick_params(axis='x', rotation=45)

# Subplot 6: Add an empty subplot (or additional plot if needed)
axs[2, 1].axis('off')  # This subplot is left empty for now

plt.tight_layout()

# Save plots and stats to a PDF
base = os.path.basename(filename).split(".csv")[0].replace("serial_data_", "")
pdf_filename = f"reports/report_{base}.pdf"

with PdfPages(pdf_filename) as pdf:
    # Add a new page with the stats table
    fig2, ax = plt.subplots(figsize=(10, 8))
    ax.axis('tight')
    ax.axis('off')
    table = ax.table(cellText=stats_df.values, colLabels=stats_df.columns, loc='center', cellLoc='center')
    table.auto_set_font_size(False)
    table.set_fontsize(10)
    table.auto_set_column_width(col=list(range(len(stats_df.columns))))
    pdf.savefig(fig2)
    plt.close(fig2)
    
    # Save the plot figure
    pdf.savefig(fig)
    plt.close(fig)

print(f"[INFO] Report saved as {pdf_filename}")
