from http.server import BaseHTTPRequestHandler, HTTPServer
import time

class Server(BaseHTTPRequestHandler):

    def do_GET(self):
        try:
            with open(self.path[1:], 'rb') as f:
                self.send_response(200)
                self.send_header('Content-type', 'text/html')
                self.end_headers()
                self.wfile.write(f.read())
        except FileNotFoundError:
            self.send_response(404)
            self.end_headers()

httpd = HTTPServer(('localhost', 3060), Server)
print('http server is starting...')
print('http server is running...')
httpd.serve_forever()
