from frame import Frame

import struct

frame_time = 0

def stream_to_frames(stream) -> list:

    global frame_time
    format_string = "<I30000B4B"
    try:
        decoded_data = struct.unpack(format_string, stream)
    except(struct.error):
        return []

    frames_list = []

    x_list = decoded_data[1:10001]
    y_list = decoded_data[10001:20001]
    z_list = decoded_data[20001:30001]
    state = decoded_data[-4]

    for i in range(10000):
        frame = Frame()

        frame.time_us = frame_time
        frame.x = x_list[i]
        frame.y = y_list[i]
        frame.z = z_list[i]
        frame.state = state

        if frame.state == 255:
            continue
        frames_list.append(frame)

        frame_time += 0.0001

    return frames_list

def read_binary_file(file_path) -> list:

    frames_list = []
    chunk_size = 30008

    with open(file_path, 'rb') as file:

        while True:

            chunk = file.read(chunk_size)
            if not chunk:
                break

            frames_list.extend(stream_to_frames(chunk))

    return frames_list
