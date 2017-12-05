#!/usr/bin/python

import MySQLdb
HOST = "localhost"
USER = "fileServerAdmin"
PASSWD = "fileServer"
DBNAME = "fileServer"

class DBConnectionException(Exception):
	def __init__( self, message ):
		super( DBConnectionException, self ).__init__( message )

class mysql_connector:

	_db_connection = None
	_cur = None

	@staticmethod
	def connect():
		_db_connection = MySQLdb.connect( HOST, USER, PASSWD, DBNAME )
		_cur = _db_connection.cursor()

	@staticmethod
	def checkCredentials( username, password ):
		if _db_connection is None:
			raise DBConnectionException("Can not connect to Database")

		
		
		
