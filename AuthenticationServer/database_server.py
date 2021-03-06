#!/usr/bin/python

import socket
import mysql_handler
import os, signal
import sys

HOST = '127.0.0.1'
PORT = 1194

socket_handler = None

def createSocketHandler():
	global socket_handler
	socket_handler = socket.socket( socket.AF_INET, socket.SOCK_STREAM )

	try:
		socket_handler.bind( ( HOST, PORT ) )

	except Exception as ex:
		print "Error opening the socket %s " % str( ex )

	socket_handler.listen( 10 )

def parseData( data ):

	index_space = data.index(' ')
	username = data[0:index_space]
	password = data[index_space+1:]
	
	return ( username, password )

def startDBServer():
	print "Starting to accept connections....."
	mysql_handler.mysql_connector.connect()
	print "Connection is established with Database server"
	while True:
		conn, addr = socket_handler.accept()
		print "Connection is established with remote client..."
		data = conn.recv( 1024 )
		username, password = parseData( data )
		is_correct = mysql_handler.mysql_connector.checkCredentials( username, password )
		if is_correct == 0:
			conn.send("correct")
		elif is_correct == 1:
			conn.send("incorrect")
		elif is_correct == 2:
			conn.send("Server error")


def interruptSignalHandler( signum, frame ):
	print "Closing connection ...."
	socket_handler.close()
	print "Terminating the programm..."
	sys.exit(0)

def main():
	signal.signal( signal.SIGINT, interruptSignalHandler )
	createSocketHandler()
	startDBServer()
	
if __name__ == "__main__":
	main()
