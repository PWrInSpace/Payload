#!/usr/bin/python3

from data_parser import read_binary_file

import sys
import time

if __name__ == "__main__":

    print("Welcome to Flash Reader for Payload 2024 - vibration measurements, version for EuRoC 2024")

    bin_file_name = sys.argv[1]
    output_file = time.strftime("payload_data_%Y-%m-%d_%H%M%S.csv")

    frames_list = read_binary_file(bin_file_name)

    with open(output_file, "w") as file:

        for frame in frames_list:
            file.write(f"{frame.toString()}\n")
