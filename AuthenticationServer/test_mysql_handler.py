#!/usr/bin/python

import threading
import mysql_handler


class thread( threading.Thread ):
	
	def run(self):
		mysql_handler.mysql_connector.connect()
		mysql_handler.mysql_connector.checkCredentials( "umit", "comodo" )




if __name__ == "__main__":

	for i in xrange( 1, 20 ):
		t = thread()
		t.start()

		
		
