package main

import (
	"fmt"
	"net"
)

func StartSocketServer(port string) (net.Listener, error) {
	// start the socket server and listen for incoming connections
	listener, err := net.Listen("tcp", ":"+port)
	if err != nil {
		return nil, fmt.Errorf("error starting socket server: %v", err)
	}
	return listener, nil
}

func AcceptClient(listener net.Listener) (net.Conn, error) {
	// accept incoming client connections
	conn, err := listener.Accept()
	if err != nil {
		return nil, fmt.Errorf("error accepting client connection: %v", err)
	}
	return conn, nil
}

func ReadFromClient(conn net.Conn) ([]byte, error) {
	// read data from the client
	buffer := make([]byte, 1024)
	n, err := conn.Read(buffer)
	if err != nil {
		return nil, fmt.Errorf("error reading from client: %v", err)
	}
	return buffer[:n], nil
}

func WriteToClient(conn net.Conn, data []byte) error {
	// write data to the client
	_, err := conn.Write(data)
	if err != nil {
		return fmt.Errorf("error writing to client: %v", err)
	}
	return nil
}

func CloseConnection(conn net.Conn) {
	// close the client connection
	conn.Close()
}
