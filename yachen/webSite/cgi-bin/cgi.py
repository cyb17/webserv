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
# content_length = os.getenv('CONTENT_LENGTH', '')
query_string = os.getenv('QUERY_STRING', '')
file_name = os.getenv('FILENAME', '')
file_body = os.getenv('FILEBODY', '')

comment_file_path = '../webPages/commentPage.html'
upload_save_directory = '../dataSubmited'

# Handling GET request
if request_method == 'GET':
	if not query_string:
		print("<html>")
		print("<body>")
		print("<h1>No query string provided. CGI Default Page is delivered.</h1>")
		print("</body>")
		print("</html>")
	else:
		# Parse the query string
		query_params = urllib.parse.parse_qs(query_string)
		name = query_params.get('name', [''])[0]
		comment = query_params.get('comment', [''])[0]
		# Print the HTML response
		print("<html>")
		print("<head>")
		print("<title></title>")
		print("</head>")
		print("<body>")
		print("<h2>CGI Transformation Page</h2>")
		print("<p>Name: {}</p>".format(name))
		print("<p>Message: {}</p>".format(message))
		print("</body>")
		print("</html>")

# Handling POST request
elif request_method == 'POST':
	if not file_body:
		print("<html><body><h1>Nothing posted so nothing to save</h1></body></html>")
	else:
		form = cgi.FieldStorage()

		if content_type == 'application/x-www-form-urlencoded':
			comment = form.getvalue('comment', '')
			try:
				with open(comment_file_path, 'a') as comment_file:
					comment_file.write(f'<p>{comment}</p>\n')
				print("<html><body><h1>submitComment successfully</h1></body></html>")
			except Exception as e:
				print(f"<html><body><h1>Error saving comment: {e}</h1></body></html>")
	elif content_type == 'multipart/form-data':
		try:
			file_path = os.path.join(upload_save_directory, file_name)
			with open(file_path, 'wb') as uploaded_file:
				uploaded_file.write(file_body)
			print("<html><body><h1>submitFile successfully</h1></body></html>")
		except Exception as e:
			print(f"<html><body><h1>Error saving file: {e}</h1></body></html>")
