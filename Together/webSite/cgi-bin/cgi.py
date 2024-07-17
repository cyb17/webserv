#!/usr/bin/env python3

import os
import cgitb
import sys
import urllib.parse

# Enable debugging
cgitb.enable()

# Get environment variables
request_method = os.getenv('REQUEST_METHOD', '')
path_info = os.getenv('PATH_INFO', '')
content_type = os.getenv('CONTENT_TYPE', '')
content_length = os.getenv('CONTENT_LENGTH', '')
query_string = os.getenv('QUERY_STRING', '')
file_name = os.getenv('FILENAME', '')
file_body = os.getenv('FILEBODY', '')

# Handling GET request
if request_method == 'GET':
	if not query_string:
		print("<html><body><h1>CGI Default Page</h1></body></html>")
	else:
		# Parse the query string
		query_params = urllib.parse.parse_qs(query_string)
		name = query_params.get('name', [''])[0]
		message = query_params.get('message', [''])[0]
		# Print the HTML response
		print("<html>")
		print("<head>")
		print("<title></title>")
		print("</head>")
		print("<body>")
		print("<h2>CGI Transform Page</h2>")
		print("<p>Name: {}</p>".format(name))
		print("<p>Message: {}</p>".format(message))
		print("</body>")
		print("</html>")

# Handling POST request
elif request_method == 'POST':
	if not file_body:
		print("<html><body><h1>Nothing to save</h1></body></html>")
	else:
		if content_type == 'application/x-www-form-urlencoded':
			print("<html><body><h1>submitComment successfully</h1></body></html>")
		elif content_type == 'multipart/form-data':
			print("<html><body><h1>submitFile successfully</h1></body></html>")
