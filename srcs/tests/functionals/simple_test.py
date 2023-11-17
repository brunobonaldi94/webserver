import socket

# Conecte-se ao servidor HTTP
host = "localhost"
port = 8080
connection = socket.create_connection((host, port))

# Envie uma solicitação HTTP inválida
request = "GET / HTTP/1.0 \r\n"
request += "Host: localhost\r\n"
request += "\r\n"  # Cabeçalhos finalizados
connection.send(request.encode())

# Leia a resposta
response = connection.recv(1024)
print(response.decode())

# Feche a conexão
connection.close()
