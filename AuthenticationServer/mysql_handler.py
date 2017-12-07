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
		mysql_connector._db_connection = MySQLdb.connect( HOST, USER, PASSWD, DBNAME )
		mysql_connector._cur = mysql_connector._db_connection.cursor()

	@staticmethod
	def checkCredentials( username, password ):
		if mysql_connector._db_connection is None:
			raise DBConnectionException("Can not connect to Database")
	
		sql = "Select Password from Users where Username='%s'" % username
		print "executes sql query is %s" % sql

		try:
			mysql_connector._cur.execute( sql )	
			result = mysql_connector._cur.fetchall()

			if not result:
				return " ";
			passwd = " "
			for row in result:
				passwd = row[0]

			if passwd == password:
				return 0
			else:
				return 1
		except Exception as ex:
			print "Exception while executing the query" + str(ex)
			return 2
			


def main():

	mysql_connector.connect()

	passwd = mysql_connector.checkCredentials( "umit", "comodo" )

	print "password is %s" % passwd


if __name__ == "__main__":
	main()
