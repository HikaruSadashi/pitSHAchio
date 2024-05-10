import socket

server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_address = ('localhost', 2000)
server_socket.bind(server_address)
server_socket.listen(5)

print("Waiting for a connection...")

while True:
    connection, client_address = server_socket.accept()

    try:
        print(f"Connection from {client_address}")

        while True:
            data = connection.recv(1024)
            if data:
                message = data.decode().strip()
                if message == "number":
                    connection.sendall(b"42")
                    print("Sent: 42")
                else:
                    connection.sendall(data)
                    print("Sent:", message)
            else:
                break

    finally:
        connection.close()
