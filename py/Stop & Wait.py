import random
import time

# Simulated network delay
def simulate_network_delay():
    return random.randint(1, 3)

# Sender function
def sender(frame):
    print(f"Sender: Sending frame {frame}")
    time.sleep(simulate_network_delay())  # Simulate network delay
    return frame

                # Receiver function
def receiver(expected_frame,frames_to_send):
    time.sleep(simulate_network_delay())  # Simulate network delay
    received_frame = expected_frame if random.random() < 0.9 else None  # Simulate packet loss
    if received_frame is not None:
        print(f"Receiver: Received frame {received_frame}")
    else:
        print(f"Receiver: Frame {expected_frame} not received")
    #print(f"Receiver : Received frame {frames_to_send}")
    return received_frame

                                                            # Main simulation loop
def main():
    frames_to_send = 5
    frame_expected = 0
    sender(0)
    while frame_expected < frames_to_send:

        ack_received = receiver(frame_expected,frames_to_send)
        if ack_received == frame_expected:
            frame_expected += 1
            sender(frame_expected)
        else:
            print("Sender: Timeout, resending frame", frame_expected)
    print(f"Receiver: Received frame {frames_to_send}")
if __name__ == "__main__":
    main()
