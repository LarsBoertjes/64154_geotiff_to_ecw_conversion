import os
import time
from datetime import datetime

INPUT_DIR = "./sample_data/input"
OUTPUT_DIR = "./sample_data/output"
LOG_FILE = "./sample_data/process.log"

def log(message):
    """Log a message with a timestamp to the log file."""
    timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    with open(LOG_FILE, "a") as log_file:
        log_file.write(f"[{timestamp}] {message}\n")

def convert_tif_to_ecw(filename):
    base = os.path.splitext(filename)[0]
    input_path = os.path.join(INPUT_DIR, filename)
    output_path = os.path.join(OUTPUT_DIR, f"{base}.ecw")

    log(f"Converting {input_path} to {output_path}")

    # GDAL conversion
    exit_code = os.system(f"gdal_translate -of E CW \"{input_path}\" \"{output_path}\"")

    if exit_code == 0:
        log(f"Successfully converted {filename} to ECW.")
    else:
        log(f"Failed to convert {filename}. Exit code: {exit_code}")

def watch_folder():
    """Watch the input directory for new .tif files and convert them to .ecw."""
    processed = set()
    while True:
        for file in os.listdir(INPUT_DIR):
            if file.endswith(".tif") and file not in processed:
                convert_tif_to_ecw(file)
                processed.add(file)
        time.sleep(5)

if __name__ == "__main__":
    os.makedirs(OUTPUT_DIR, exist_ok=True)
    os.makedirs(os.path.dirname(LOG_FILE), exist_ok=True)
    log("Starting folder watch service.")
    watch_folder()

