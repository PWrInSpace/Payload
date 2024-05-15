#!/usr/bin/python3

from data_parser import read_binary_file
from serial_reader import read_from_serial

import sys
import time

if __name__ == "__main__":

    print("Welcome to Flash Reader for Payload 2024 - vibration measurements")

    input_stream = sys.argv[1]
    serial_mode = False
    bin_file_name = time.strftime("binary_payload_data_%Y-%m-%d_%H%M%S.bin")

    if "/dev/tty" in input_stream or "COM" in input_stream:
        read_from_serial(input_stream, bin_file_name)
        serial_mode = True

    output_files = [time.strftime("payload_slot_A_%Y-%m-%d_%H%M%S.csv"), "payload_slot_B_%Y-%m-%d_%H%M%S.csv"]

    content_dict = read_binary_file(bin_file_name if serial_mode else input_stream)

    slot_a_list = content_dict["SLOT_A"]
    with open(output_files[0], "w") as file:

        for frame in slot_a_list:
            file.write(f"{frame.toString()}\n")

    slot_b_list = content_dict["SLOT_B"]
    with open(output_files[1], "w") as file:

        for frame in slot_b_list:
            file.write(f"{frame.toString()}\n")
