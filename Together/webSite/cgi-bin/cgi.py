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

# Parse the query string
query_params = urllib.parse.parse_qs(query_string)
name = query_params.get('name', [''])[0]
message = query_params.get('message', [''])[0]

# Print the HTML response
print("<html>")
print("<head>")
print("<title>CGI POST Example</title>")
print("</head>")
print("<body>")
print("<h2>Received GET Data</h2>")
print("<p>Name: {}</p>".format(name))
print("<p>Message: {}</p>".format(message))
print("<h2>Environment Variables</h2>")
print("<p>REQUEST_METHOD: {}</p>".format(request_method))
print("<p>PATH_INFO: {}</p>".format(path_info))
print("<p>CONTENT_TYPE: {}</p>".format(content_type))
print("<p>CONTENT_LENGTH: {}</p>".format(content_length))
print("<p>QUERY_STRING: {}</p>".format(query_string))
print("</body>")
print("</html>")
