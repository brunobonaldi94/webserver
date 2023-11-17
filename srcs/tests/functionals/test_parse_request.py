import unittest
import socket

class TestParseRequest(unittest.TestCase):
    def setUp(self) -> None:
        self.host = 'localhost'
        self.port = 8080
        self.connection = socket.create_connection((self.host, self.port))
    
    def _send(self, request):
        self.connection.send(request.encode())
        return str(self.connection.recv(1024))

    
    def test_prototol_name(self):
        #send a protocol name invalid
        self.assertIn('Bad Request', self._send("GET / HTP/1.1 \r\n"))
        self.assertIn('Bad Request', self._send("GET / HHTTP/1.1 \r\n"))
        self.assertIn('Bad Request', self._send("GET / HTTP/1. \r\n"))
        self.assertIn('Bad Request', self._send("GET / HTTP/1.1.1 \r\n"))
        self.assertIn('Bad Request', self._send("GET / HTTP/ \r\n"))
        self.assertIn('Bad Request', self._send("GET / HTTP \r\n"))
        self.assertIn('Bad Request', self._send("GET / HTTP/.1 \r\n"))

        self.assertIn('OK', self._send("GET / HTTP/1.1 \r\n"))
    
    def test_prototol_version(self):
        #send a protocol version invalid
        self.assertIn('HTTP Version Not Supported', self._send("GET / HTTP/1.0 \r\n"))
        self.assertIn('HTTP Version Not Supported', self._send("GET / HTTP/0.9 \r\n"))
        self.assertIn('HTTP Version Not Supported', self._send("GET / HTTP/2.0 \r\n"))
        self.assertIn('HTTP Version Not Supported', self._send("GET / HTTP/3.0 \r\n"))

        self.assertIn('OK', self._send("GET / HTTP/1.1 \r\n"))
    
    def tearDown(self) -> None:
        self.connection.close()

if __name__ == '__main__':
    unittest.main()
        
    
#python -m unittest srcs/tests/functionals/test_parse_request.py 