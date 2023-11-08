import http.client
import sys
import time

print('input file name (ex. test.html): ', end='')
file_name = input()
print("Connection establishing")
while file_name.lower() != 'exit':
    conn = http.client.HTTPConnection("localhost", 3060)
    conn.request("GET", "/" + file_name)

    resp = conn.getresponse()
    print(resp.status, resp.reason)

    data = resp.read()
    print(data.decode())

    conn.close()
    print("Connection terminated...")
    time.sleep(3)
    print('input file name (ex. test.html): ', end='')
    file_name = input()

print('Exiting...')
