import random
import time
random.seed(9821)
WINDOW_SIZE = 3  # Window size for the sender and receiver
MAX_FRAMES = 10  # Total number of frames to be sent

# Simulated network delay
def simulate_network_delay():
    return random.randint(1, 3)

# Sender function
def sender(frames):
    base = 0
    next_seq_num = 0
    buffer = frames[0:WINDOW_SIZE]

    while base < len(frames):
        for i in range(base, min(base + WINDOW_SIZE, len(frames))):
            print(f"Sender: Sending frame {frames[i]}")
            time.sleep(simulate_network_delay())  # Simulate network delay


        temp = buffer
        for i in range(base, min(base + WINDOW_SIZE, len(frames))):
            ack_received = receiver(base, buffer)

            if ack_received == base:
                base += 1
                buffer.pop(0)
                if next_seq_num < len(frames):
                    buffer.append(frames[next_seq_num])
                    next_seq_num += 1
            else:
                buffer = temp
                print(f"Sender: Timeout, resending frames [{base}:{base+WINDOW_SIZE-1}]")
                break

# Receiver function
def receiver(expected_seq_num, buffer):
    time.sleep(simulate_network_delay())  # Simulate network delay
    received_frame = expected_seq_num if random.random() < 0.75 else None  # Simulate packet loss

    if received_frame is not None:
        print(f"Receiver: Received frame {received_frame}")
        if received_frame == expected_seq_num:
            return received_frame

    return None

def main():
    frames_to_send = list(range(MAX_FRAMES))
    sender(frames_to_send)

if __name__ == "__main__":
    main()
