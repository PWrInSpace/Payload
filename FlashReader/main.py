#!/usr/bin/python3

import struct

class Frame:

    time_us = 0
    x = 0
    y = 0
    z = 0
    state = 0

    def toString(self):
        return f"{self.time_us}; {self.x}; {self.y}; {self.z}; {self.state}"

frame_time = 0

def stream_to_frames(stream) -> Frame:

    global frame_time
    format_string = "<I2040B4B"
    decoded_data = struct.unpack(format_string, stream)

    frames_list = []

    for i in range(680):
        frame = Frame()

        frame.time_us = frame_time
        frame.x = decoded_data[i * 3 + 1]
        frame.y = decoded_data[i * 3 + 2]
        frame.z = decoded_data[i * 3 + 3]
        frame.state = decoded_data[-4]

        if frame.state == 255:
            continue
        frames_list.append(frame)

        frame_time += 0.0001

    return frames_list

def read_binary_file(file_path) -> dict:

    data_dict = {}
    current_slot = None

    with open(file_path, 'rb') as file:
        
        file_content = file.read()

    if file_content[0:6] == b"SLOT_A":
        
        current_slot = "SLOT_A"
        data_dict[current_slot] = []
        file_content = file_content[6:]
        print("Parsing SLOT_A")

        while len(file_content) > 0:

            if file_content[0:6] == b"SLOT_B":
                current_slot = "SLOT_B"
                data_dict[current_slot] = []
                file_content = file_content[6:]
                print("Parsing SLOT_B")
            
            else:
                stream = file_content[0:2048]
                measurements_list = stream_to_frames(stream)
                data_dict[current_slot].extend(measurements_list)
                file_content = file_content[2048:]
    
    else:
        print("Error, no SLOT_A")

    return data_dict

if __name__ == "__main__":

    print("Welcome to Flash Reader for Payload 2024 - vibration measurements")

    input_file = "cutecom.log"
    output_files = ["outA.csv", "outB.csv"]

    content_dict = read_binary_file(input_file)

    slot_a_list = content_dict["SLOT_A"]
    with open(output_files[0], "w") as file:

        for frame in slot_a_list:
            file.write(f"{frame.toString()}\n")

    slot_b_list = content_dict["SLOT_B"]
    with open(output_files[1], "w") as file:

        for frame in slot_b_list:
            file.write(f"{frame.toString()}\n")
