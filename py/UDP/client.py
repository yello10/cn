import socket


def client_program():
    client_socket = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
    server_address = ('127.0.0.1',3035)

    while True:
        message = input("Enter the message to send (or stop to quit): ")
        client_socket.sendto(message.encode('utf-8'),server_address)
        if message =="stop":
            print("Ending connection")
            break
        response, server_address = client_socket.recvfrom(1024)
        print(f"Received from server: {response.decode('utf-8')}")
    client_socket.close()

if __name__=="__main__":
    client_program()
