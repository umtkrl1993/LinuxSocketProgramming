


import socket
import os
from threading import Thread

HOST = "0.0.0.0"
PORT = 9876
OVA_SCRIPT_PATH="/home/umit/VirtualBox\ VMs/vmcreator.sh"

def createSocketHandler():
	global socket_handler
	socket_handler = socket.socket( socket.AF_INET, socket.SOCK_STREAM )

	try:
		socket_handler.bind( ( HOST, PORT ) )

	except Exception as ex:
		print "Error opening the socket %s " % str( ex )
		exit(0)

	socket_handler.listen( 10 )


def scriptExecutor( conn, addr ):
	import subprocess

	data = conn.recv(1024)

	if data:
		print "Parent pid is %d" %os.getpid()
		#pid = os.fork()
		subprocess.call(['bash','dummy.sh'])
		args = ( "bash", "dummy.sh" )
		print  "Child pid is %d" %os.getpid()
		#os.execvp("bash", args )
	

		#os.waitpid( pid, 0)




def startOVAServer():
	global socket_handler

	while True:
		conn, addr = socket_handler.accept()

		thread = Thread( target = scriptExecutor, args = ( conn, addr ) )

		thread.start()
		thread.join()





def main():
	createSocketHandler()
	startOVAServer()


if __name__ == "__main__":
	main()
