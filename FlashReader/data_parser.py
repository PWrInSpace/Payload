from frame import Frame

import struct

frame_time = 0

def stream_to_frames(stream) -> Frame:

    global frame_time
    format_string = "<I2040B4B"
    try:
        decoded_data = struct.unpack(format_string, stream)
    except(struct.error):
        return []

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

        frame_time += 0.00025

    return frames_list

def read_binary_file(file_path) -> dict:

    data_dict = {}
    current_slot = None

    with open(file_path, 'rb') as file:
        
        file_content = file.read()

    if True: # file_content[0:6] == b"SLOT_A":
        
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
