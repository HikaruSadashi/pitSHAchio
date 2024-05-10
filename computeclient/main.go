package main

import (
	"fmt"
	"net"
	"crypto/sha256"
	"encoding/hex"
	"strings"
	"time"
)

// incrementStr increments a string by one character
func incrementStr(str string) string {
	runes := []rune(str)
	for i := len(runes) - 1; i >= 0; i-- {
		if runes[i] == '9' {
			runes[i] = 'A'
			break
		} else if runes[i] == 'Z' {
			runes[i] = 'a'
			break
		} else if runes[i] == 'z' {
			runes[i] = '0'
		} else {
			runes[i]++
			break
		}
	}
	if runes[0] == 0 {
		runes = append([]rune{'0'}, runes...)
	}
	return string(runes)
}

// intToHex converts an integer to its hexadecimal ASCII character representation
func intToHex(x int) byte {
	y := x + 48
	if y > 57 {
		y += 39
	}
	return byte(y)
}

// hashPassword computes the SHA-256 hash of a password and returns it as a hexadecimal string
func hashPassword(password string) string {
	hasher := sha256.New()
	hasher.Write([]byte(password))
	hashedPassword := hasher.Sum(nil)
	return hex.EncodeToString(hashedPassword)
}

func main() {
	ip := "127.0.0.1"
	if len(os.Args) > 1 {
		ip = os.Args[1]
	}

	for {
		conn, err := net.Dial("tcp", ip+":3010")
		if err != nil {
			fmt.Println("error connecting to server:", err)
			time.Sleep(3 * time.Second) // Wait before retrying connection
			continue
		}

		defer conn.Close()

		// Read the password hash from the server
		buffer := make([]byte, 1024)
		n, err := conn.Read(buffer)
		if err != nil || n == 0 {
			fmt.Println("server closed")
			break
		}
		pwHash := strings.TrimSpace(string(buffer[:n]))

		fmt.Println("cracking", pwHash)

		complete := false
		_, err = conn.Write([]byte{20}) // Send ready signal to server
		if err != nil {
			fmt.Println("error writing to server:", err)
			continue
		}

		for {
			n, err := conn.Read(buffer)
			if err != nil || n == 0 {
				complete = true
				break
			}

			incoming := strings.TrimSpace(string(buffer[:n]))
			if incoming == "" || incoming[0] == 22 {
				incoming = ""
			}
			fmt.Println("attempting strings of class", incoming)

			component := ""
			for len(component) < 4 {
				h := sha256.New()
				sum := incoming + component
				h.Write([]byte(sum))
				hashed := h.Sum(nil)

				hash := make([]byte, hex.EncodedLen(len(hashed)))
				hex.Encode(hash, hashed)

				if strings.Compare(hex.EncodeToString(hashedPassword), pwHash) == 0 {
					conn.Write([]byte(sum))
					complete = true
					break
				}
				component = incrementStr(component)
			}
			if complete {
				break
			}

			conn.Write([]byte{10}) // Send response to server
		}
		time.Sleep(5 * time.Second) // Sleep before reconnecting to server
	}
}
