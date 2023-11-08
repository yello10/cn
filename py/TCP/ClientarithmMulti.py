import socket


def client_program():
    client_socket = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
    server_address = ('127.0.0.1',3040)
    client_socket.connect(server_address)
    while True:
        inp = input("Enter the arithmetic expression")
        if inp=="stop":
            break
        client_socket.send(inp.encode())
        data = client_socket.recv(1024).decode()
        print("Received:",data)

    client_socket.close()
if __name__ == "__main__":
    client_program()