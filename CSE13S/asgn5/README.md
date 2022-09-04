# Assignment 5: Public Key Cryptography

## Description:

	Performs RSA encryption in three phases. Keygen creates the public and private keys store in separate files to be used by the encryption phase. This phase encrypts any files and outputs their RSA level encryption. Decryption phase decrypts files inputted and retursn them to their unaltered state before encryption.

## Build:

	$ make all
	This will build all three exacutables.
	You can also build them separatly with $ make keygen, $ make decrypt and $ make encrypt.

## Running:
	
	./keygen -i[#] -b[#] -n[""] -d[""] -s[#] -v -h

	./encrypt -i[""] -o[""] -n[""] -v -h

	./decrypt -i[""] -o[""] -n[""] -v -h

## Cleaning:
	
	$ make clean
	This will remove all the files genearted from $ make all.

## Command-Line Keygen options:

| Argument | Description                                                                                   |
|----------|-----------------------------------------------------------------------------------------------|
| -b       | specifies the minimum bits needed for the public modulus n.                                   |
| -i       | specifies the number of Miller-Rabin iterations for testing primes (default: 50).             |
| -n       | specifies the public key file (default: rsa.pub).                                             |
| -d       | specifies the private key file (default: rsa.priv).                                           |
| -s       | specifies the random seed for the random state initialization (default: time(NULL)) 	   |
| -v       | enables verbose output.                                                                       |
| -h       | displays program synopsis and usage.                                                          |

## Command-Line Encrypt options:

| Argument | Description                                                      |
|----------|------------------------------------------------------------------|
| -i       | specifies the input file to encrypt (default: stdin).            |
| -o       | specifies the output file to encrypt (default: stdout).          |
| -n       | specifies the file containing the public key (default: rsa.pub). |
| -v       | enables verbose output.                                          |
| -h       | displays program synopsis and usage.                             |

## Command-Line Decrypt options:

| Argument | Description                                                        |
|----------|--------------------------------------------------------------------|
| -i       | specifies the input file to encrypt (default: stdin).              |
| -o       | specifies the output file to encrypt (default: stdout).            |
| -n       | specifies the file containing the private key (default: rsa.priv). |
| -v       | enables verbose output.                                            |
| -h       | displays program synopsis and usage.                               |
