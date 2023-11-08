import http.client
import sys

print('input file name (ex. test.html): ', end='')
file_name = input()
print("Connection established")
conn = http.client.HTTPConnection("localhost",3045)
while file_name.lower() != 'exit':
    conn.request("GET", "/" + file_name)

    resp = conn.getresponse()
    print(resp.status, resp.reason)

    data = resp.read()
    print(data.decode())


    print('input file name (ex. test.html): ', end='')
    file_name = input()
conn.close()
print('Connection terminated and Exiting...')
