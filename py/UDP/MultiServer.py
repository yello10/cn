import socket
import threading

HOST = '127.0.0.1'
PORT = 3039

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((HOST, PORT))

print('Server listening on port', PORT)

def handle_client(data, addr):
  a,op,b = data.decode().split()

  if a == "stop" or b == "stop":
    print(f'Client {addr} disconnected')
    return

  a = int(a)
  b = int(b)
  result=0
  if op == '+':
    result = a + b
  elif op == '-':
    result = a - b

  sock.sendto(str(result).encode(), addr)

while True:

  data, addr = sock.recvfrom(1024)

  thread = threading.Thread(target=handle_client, args=(data, addr))
  thread.start()