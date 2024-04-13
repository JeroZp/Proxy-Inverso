import socket

def send_and_receive(server_ip, server_port, message, output_file, sent_message_file):
    # Create a socket object
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        # Connect to the server
        s.connect((server_ip, server_port))
        
        # Send data (encode string to bytes)
        s.sendall(message.encode())
        
        # Save the sent message to a file
        with open(sent_message_file, 'w') as f_sent:
            f_sent.write(message)
        print("Message sent and saved to file:", sent_message_file)
        
        # Receive data from the server
        data = s.recv(1024)
        
        # Decode bytes to string
        received_message = data.decode()
        
        # Write the received data to a file
        with open(output_file, 'w') as f_received:
            f_received.write(received_message)
        print("Data received and saved to file:", output_file)

# Example usage
server_ip = '192.168.1.2'  # Replace with the server's IP address
server_port = 12345        # Replace with the server's port number
message = input('Enter the message to send: ')  # Get user input for the message
output_file = 'response.txt'  # File to save the response
sent_message_file = 'sent_message.txt'  # File to save the sent message

send_and_receive(server_ip, server_port, message, output_file, sent_message_file)
