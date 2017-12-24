#!/usr/bin/python

import MySQLdb
import threading

HOST = "localhost"
USER = "fileServerAdmin"
PASSWD = "fileServer"
DBNAME = "fileServer"

db_mutex = threading.Lock()

class DBConnectionException(Exception):
	def __init__( self, message ):
		Exception.__init__( self, message )

class mysql_connector:

	_db_connection = None
	_cur = None

	@staticmethod
	def connect():
		if mysql_connector._db_connection is None:
			mysql_connector._db_connection = MySQLdb.connect( HOST, USER, PASSWD, DBNAME )
			mysql_connector._cur = mysql_connector._db_connection.cursor()

	@staticmethod
	def checkCredentials( username, password ):
		if mysql_connector._db_connection is None:
			raise DBConnectionException("Can not connect to Database")
	
		global db_mutex
		db_mutex.acquire()
		current_threadID = threading.current_thread()
		print "Thread %s got the lock" %str( current_threadID )
		sql = "Select Password from Users where Username='%s'" % username
		print "executes sql query is %s" % sql

		try:
			mysql_connector._cur.execute( sql )	
			result = mysql_connector._cur.fetchall()
			return_value = " ";
			if not result:
				db_mutex.release()
				return " "

			passwd = result[0][0]

			if passwd == password:
				return_value = 0
			else:
				return_value = 1
		except Exception as ex:
			print "Exception while executing the query" + str(ex)
			return_value = 3

		
		print "Thread %s is releasing the lock" %str( current_threadID )
		db_mutex.release()
		return return_value
			


def main():

	mysql_connector.connect()

	passwd = mysql_connector.checkCredentials( "umit", "comodo" )

	print "password is %s" % passwd


if __name__ == "__main__":
	main()
