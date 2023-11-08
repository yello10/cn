import socket


def server_program():
    server_socket = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
    server_address = ('127.0.0.1',3032)
    server_socket.bind(server_address)
    server_socket.listen()
    conn,addr = server_socket.accept()  #we'll get the conn object and the client address
    with conn:
        print('Connected by ',addr)
        while True:
            data = conn.recv(1024)
            if not data:
                break
            a,op,b = data.decode().split()
            if a=="stop" or b=="stop" or op=="":
                print("Connection terminated")
                break
            a = int(a)
            b = int(b)
            result=0
            if op=='+':
                result = a+b
            elif op=='-':
                result = a-b
            elif op=='*':
                result = a*b
            else:
                result = a/b
            conn.sendall(str(result).encode())

    print("Connection finally terminated")
    server_socket.close()


if __name__ =="__main__":
    server_program()