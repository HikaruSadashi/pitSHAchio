package main

import (
	"fmt"
	"net"
	"crypto/sha256"
	"encoding/hex"
)

func main() {
	fmt.Println("starting PitSHAchio compute server...")

	// start listening for incoming connections on port 3010
	server, err := net.Listen("tcp", ":3010")
	if err != nil {
		fmt.Println("error starting server:", err)
		return
	}
	defer server.Close()

	// accept and handle incoming connections
	for {
		conn, err := server.Accept()
		if err != nil {
			fmt.Println("error accepting connection:", err)
			continue
		}

		// handle each incoming connection concurrently
		go handleConnection(conn)
	}
}

func handleConnection(conn net.Conn) {
	defer conn.Close()

	// read the input from the client
	buf := make([]byte, 1024)
	n, err := conn.Read(buf)
	if err != nil {
		fmt.Println("error reading from connection:", err)
		return
	}
	input := string(buf[:n])

	// compute the hash of the input
	hash := HashPassword(input)

	// send the computed hash back to the client
	_, err = conn.Write([]byte(hash))
	if err != nil {
		fmt.Println("error writing to connection:", err)
		return
	}
}

func HashPassword(password string) string {
	// compute the SHA-256 hash of the password
	hasher := sha256.New()
	hasher.Write([]byte(password))
	hashedPassword := hasher.Sum(nil)

	// convert the hashed password to a hexadecimal string
	return hex.EncodeToString(hashedPassword)
}
