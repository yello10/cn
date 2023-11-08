import socket
HOST = '127.0.0.1'
PORT = 3037
FORMAT = 'utf-8'
SIZE = 1024
def main():
    client = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
    client.connect((HOST,PORT))

    #opening and reading file data
    file = open("content/data.txt", "r")
    data = file.read()

    #sending the filename to the server
    client.send("content/data.txt".encode(FORMAT))
    msg = client.recv(SIZE).decode(FORMAT)
    print(f"Server: {msg}")

    #Closing the file
    file.close()

    client.close()
if __name__ == "__main__":
    main()