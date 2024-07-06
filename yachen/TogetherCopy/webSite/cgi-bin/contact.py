#!/usr/bin/env python

import cgi
import cgitb
cgitb.enable()  # Pour le débogage

print("Content-Type: text/html\n")

form = cgi.FieldStorage()

name = form.getvalue("name")
message = form.getvalue("message")

print("<html><body>")
print("<h1>Merci pour votre soumission</h1>")
print("<p>Nom: {}</p>".format(name))
print("<p>Message: {}</p>".format(message))
print("</body></html>")
