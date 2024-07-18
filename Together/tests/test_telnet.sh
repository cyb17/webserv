#!/bin/bash

HOST="localhost"
PORT=8080

echo "host : $HOST" 
echo "port : $PORT"
echo 
echo "Test 1: une requête POST"
echo "-------------------------------"
echo "POST /webPages/submitComment.html HTTP/1.1\r\n"
echo "Host: localhost\r\n"
echo "Content-Type: application/x-www-form-urlencoded\r\n"
echo "Content-Length: 21\r\n"
echo ""
echo "name=jo&comment=hello"
echo "-------------------------------"

# Utiliser telnet pour envoyer la requête
{
	echo "POST /webPages/submitComment.html HTTP/1.1"
	echo "Host: localhost"
	echo "Content-Type: application/x-www-form-urlencoded"
	echo "Content-Length: 21"
	echo ""
	echo -n "name=jo&comment=hello"
	sleep 5
} | telnet $HOST $PORT
