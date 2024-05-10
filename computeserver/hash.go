package main

import (
	"crypto/sha256"
	"encoding/hex"
)

// (using SHA-256)
func HashPassword(password string) string {
	
	hasher := sha256.New()

	hasher.Write([]byte(password)) // write the password bytes to the hasher
	hashedPassword := hasher.Sum(nil) // get bytes	
	return hex.EncodeToString(hashedPassword) //to hexadecimal string
}
