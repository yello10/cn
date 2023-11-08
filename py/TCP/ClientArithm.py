import socket


def client_program():
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_address = ('127.0.0.1', 3032)
    client_socket.connect(server_address)
    while True:
        inp = input("Enter the expression or stop to exit: ")
        if inp == "stop":
            print("Connection terminated")
            break
        client_socket.sendall(inp.encode())
        data = client_socket.recv(1024)
        print('Received from server: ', data.decode())

    client_socket.close()


if __name__ == "__main__":
    client_program()
