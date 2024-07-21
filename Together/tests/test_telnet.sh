#!/bin/bash

BOLD='\033[1m'
RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m'

echo "${BLUE}${BOLD}--------------------------------------------"
echo "|                                          |"
echo "|  TESTING METHODS: GET, POST AND DELETE   |"
echo "|											 |"
echo "--------------------------------------------${NC}"

PORT="8080"
HOST="localhost"

echo 
echo "${BOLD}INFOS:${NC}"
echo "* Host : $HOST" 
echo "* Port : $PORT"
echo
echo "===================================================="
echo 
echo "               ${BLUE}TESTING METHOD: GET${NC}      "
echo
echo "    ${BOLD}* TEST 1: Display a HTML file${NC} 	  "
echo "    -------------------------------------------     "
echo "    GET /webPages/file2.html HTTP1.1\r\n			  "
echo "	  Host: $HOST:$PORT\r\n					    	  "
echo "	  \r\n                                            "
echo "    -------------------------------------------     "	
echo
printf "${BOLD}Server response:\n${NC}"
{
	printf "GET /webPages/ HTTP1.1\r\n"		
	printf "Host: $HOST:$PORT\r\n"
	printf "\r\n"
	sleep 3
} | telnet $HOST $PORT
if [ $? -eq 0 ]; then
    printf "\n${GREEN}${BOLD}SUCCESS! :)\n\n${NC}"
else
    printf "\n${RED}${BOLD}SORRY, EXPECTED STATUS CODE = 200... :(\n\n${NC}"
fi

echo "    ${BOLD}* TEST 2: Display the index${NC}         "
echo "    -------------------------------------------     "
echo "    GET / HTTP1.1\r\n								  "
echo "	  Host: $HOST:$PORT\r\n  						  "
echo "	  \r\n                                            "
echo "    -------------------------------------------     "	
echo
printf "${BOLD}Server response:\n${NC}"
{
	printf "GET / HTTP1.1\r\n"		
	printf "Host: $HOST:$PORT\r\n"
	printf "\r\n"
	sleep 3
} | telnet $HOST $PORT
if [ $? -eq 0 ]; then
    printf "\n${GREEN}${BOLD}SUCCESS! :)\n\n${NC}"
else
    printf "\n${RED}${BOLD}SORRY, EXPECTED STATUS CODE = 200... :(\n\n${NC}"
fi

echo "    ${BOLD}* TEST 3: Display a listing${NC}         "
echo "    -------------------------------------------     "
echo "    GET /webPages/ HTTP1.1\r\n					  "
echo "	  Host: $HOST:$PORT\r\n					    	  "
echo "	  \r\n                                            "
echo "    -------------------------------------------     "	
echo
printf "${BOLD}Server response:\n${NC}"
{
	printf "GET /webPages/ HTTP1.1\r\n"		
	printf "Host: $HOST:$PORT\r\n"
	printf "\r\n"
	sleep 3
} | telnet $HOST $PORT
if [ $? -eq 0 ]; then
    printf "\n${GREEN}${BOLD}SUCCESS! :)\n\n${NC}"
else
    printf "\n${RED}${BOLD}SORRY, EXPECTED STATUS CODE = 200... :(\n\n${NC}"
fi

echo "    ${BOLD}* TEST 4: Submit Comment${NC}            "
echo "    -------------------------------------------     "
echo "    GET /webPages/submitComment.html?name=Ya&comment=Hello HTTP1.1\r\n	  "
echo "	  Host: $HOST:$PORT\r\n						  "
echo "	  \r\n                                            "
echo "    -------------------------------------------     "	
echo
printf "${BOLD}Server response:\n${NC}"
{
	printf "GET /webPages/submitComment.html?name=Ya&comment=Hello HTTP1.1\r\n"
	printf "Host: $HOST:$PORT\r\n"
	printf "\r\n"
	sleep 3
} | telnet $HOST $PORT
if [ $? -eq 0 ]; then
    printf "\n${GREEN}${BOLD}SUCCESS! :)\n\n${NC}"
else
    printf "\n${RED}${BOLD}SORRY, EXPECTED STATUS CODE = 200... :(\n\n${NC}"
fi

echo "Test 1: une requête POST"
echo "-------------------------------"
echo "POST /webPages/submitComment.html HTTP/1.1\r\n"
echo "Host: localhost\r\n"
echo "Content-Type: application/x-www-form-urlencoded\r\n"
echo "Content-Length: 21\r\n"
echo 
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
