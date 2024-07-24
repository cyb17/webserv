#!/usr/bin/env python3

import os
import cgi
import cgitb
import sys
import urllib.parse

# Enable debugging
cgitb.enable()

# Get environment variables
# path_info = os.getenv('PATH_INFO', '')
# content_length = os.getenv('CONTENT_LENGTH', '')
request_method = os.getenv('REQUEST_METHOD', '')
content_type = os.getenv('CONTENT_TYPE', '')
query_string = os.getenv('QUERY_STRING', '')
file_name = os.getenv('FILENAME', '')
file_body = os.getenv('FILEBODY', '')

current_directory = os.getcwd()
comment_file_path = os.path.join(current_directory, 'webSite/cgi-bin/dataSubmited/commentPage.html')
upload_save_directory = os.path.join(current_directory, 'webSite/cgi-bin/dataSubmited/')


def parse_urlencoded(content):
	lines = content.split('\n')
	comments = []	
	for line in lines:
		if line.strip():  # Ignorer les lignes vides
			parts = line.split('&')
			comment_data = {}
			for part in parts:
				key, value = part.split('=')
				comment_data[key] = value
			comments.append(comment_data)

	return comments

def generate_html(comments):
	html_content = """
	<!DOCTYPE html>
	<html lang="en">
	<head>
		<meta charset="UTF-8">
		<meta name="viewport" content="width=device-width, initial-scale=1.0">
		<title>Comments</title>
		<style>
			body
			{
				font-family: Arial, sans-serif;
				background-color: #f4f4f9;
			}
			.comment { margin-bottom: 20px; }
			.name
			{
				font-weight: bold;
				color: #007BFF;
			}
			.comment-text { margin-left: 20px; }
		</style>
	</head>
	<body>
	"""

	for comment in comments:
		html_content += f"""
		<div class="comment">
			<div class="name">{comment['name']}:</div>
			<div class="comment-text">{comment['comment']}</div>
		</div>
		"""	
	html_content += """
	</body>
	</html>
	"""

	return html_content


def convert_file_to_html(file_path):
	with open(file_path, 'r', encoding='utf-8') as file:
		content = file.read()
	comments = parse_urlencoded(content)
	html = generate_html(comments)
	print(html)


# Handling GET request
if request_method == 'GET':
	if not query_string:
		print("<html><body><h1>CGI Default page is delivered</h1></body></html>")
	elif query_string:
		query_params = urllib.parse.parse_qs(content)
		name = query_params.get('name', [''])[0]
		comment = query_params.get('comment', [''])[0]
		print("<html><body>")
		print("<p>Name: {}</p>".format(name))
		print("<p>Message: {}</p>".format(comment))
		print("</body></html>")

# Handling POST request
elif request_method == 'POST':
	if not file_body:
		print("<html><body><h1>Nothing posted so nothing to save</h1></body></html>")
	else:
		if content_type == 'application/x-www-form-urlencoded':
			try:
				with open(comment_file_path, 'a') as comment_file:
					comment_file.write(file_body + '\n')
				print("<html><body><h1>Comment submited successfully</h1></body></html>")
				convert_file_to_html(comment_file_path)
			except Exception as e:
				print(f"<html><body><h1>Error saving comment: {e}</h1></body></html>")
		elif content_type == 'multipart/form-data':
			if isinstance(file_body, str):
				file_body = file_body.encode('utf-8')
			try:
				file_path = os.path.join(upload_save_directory, file_name)
				with open(file_path, 'wb') as uploaded_file:
					uploaded_file.write(file_body)
				print("<html><body><h1>File submited successfully</h1></body></html>")
			except Exception as e:
				print(f"<html><body><h1>Error saving file: {e}</h1></body></html>")
				
