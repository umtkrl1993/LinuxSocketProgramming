import socket
import sys
from thread import*
def messageReceiverThread( sock ):
        
        while True:
                    received_data = sock.recv( 1024 )
                            print "Server message " + received_data

                            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

                            server_address = ("127.0.0.1", 8888 )
                            sock.connect(server_address)

                            start_new_thread( messageReceiverThread, ( sock, ) )
                            try:
                                    while True:
                                                message = raw_input( "Please enter a message to server " )
                                                        sock.sendall( message )

                            finally:
                                sock.close()
