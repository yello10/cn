import socket

def server_program():
    server_socket = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
    server_address = ('127.0.0.1',3035)
    server_socket.bind(server_address)
    while True:
        data,clientaddr = server_socket.recvfrom(1024)
        message = data.decode('utf-8')
        if message == "stop":
            print("End of connection")
            break
        response = "OK"
        server_socket.sendto(response.encode('utf-8'),clientaddr)

    server_socket.close()
if __name__ =="__main__":
    server_program()