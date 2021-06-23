
import socket
import sys


def main():
	portNumber = sys.argv[1]					
	host ='localhost'							
	portNumber = int(portNumber)				
	print("Welcome to Client Server Messaging Application\n")
	sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	sock.bind(('localhost', portNumber))
	sock.listen(1)
	print("Waiting on Port", portNumber)

	while True:
		connection, clientAddress = sock.accept()	
		print('Connect to Client: ', clientAddress)		
		while True:
			handle = b''												
			while len(handle) < 11:										
				handle += connection.recv(11)
			handle = handle.decode('utf-8')								
			message = b''												
			while len(message) < 500:									
				message += connection.recv(500)
			message = message.decode('utf-8')						

			if (message[0] == "q" and message[1] == "u" and message[2] == "i" and message[3] == "t"):
				print("Connection is Terminated By Client\n")
				exit()

			print(handle + ": ", end="")								
			print(message)												

			response = input("Your Message: ")					
			if (response == "quit"):							
				connection.sendall(response.encode('utf-8'))			
				print("Terminating Connection\n")
				exit()

			connection.sendall(response.encode('utf-8'))			
			print("Waiting response from "+handle)

main()
