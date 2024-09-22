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

    for i in range(10000):
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
