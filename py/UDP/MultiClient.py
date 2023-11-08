import socket

HOST = '127.0.0.1'
PORT = 3039

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

while True:
  input_str = input('Enter operation (a op b) or "stop" to quit: ')

  if input_str == 'stop':
    break

  sock.sendto(input_str.encode(), (HOST, PORT))

  result, stuff = sock.recvfrom(1024)
  print('Result:', result.decode())

sock.close()