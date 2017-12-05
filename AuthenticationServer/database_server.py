#!/usr/bin/python

import socket
import mysql_handler

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
	username = data[0:2]
	password = data[3:4]

	return ( username, password )

def startDBServer():
	mysql_handler.mysql_connector.connect()
	while True:
		conn, addr = socket_handler.accept()
		data = conn.recv( 1024 )
		username, password = parseData( data )
		is_correct = mysql_handler.mysql_connector.checkCredentials( username, password )
		if is_correct == True:




