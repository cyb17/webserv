#!/bin/bash

BOLD='\033[1m'
HIGHLIGHT='\033[7m'
UNDERLINE='\033[4m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;36m'
NC='\033[0m'

PORT="8080"
HOST="localhost"

function gettest1 {
	printf "            ${YELLOW}___________________________\n"
	printf "            |                         |\n"
	printf "            |   TESTING METHOD: GET   |\n"
	printf "            |_________________________|\n\n\n${NC}"
	printf "====================================================\n\n\n"
	printf "${BLUE}${BOLD}${UNDERLINE}* TEST 1: DISPLAY A HTML FILE\n\n\n${NC}"
	printf "${BOLD}${UNDERLINE}REQUEST\n${NC}"
	printf "___________________________________________________\n\n"
	printf "GET /webPages/file2.html HTTP/1.1\\\r\\\n	         \n"
	printf "Host: %s:%s\\\r\\\n\n" "$HOST" "$PORT"
	printf "\\\r\\\n                                             \n"
	printf "___________________________________________________${NC}\n\n\n"	
	printf "${BOLD}${UNDERLINE}RESPONSE\n${NC}"
	printf "___________________________________________________\n\n"
	response=$( {
		echo "GET /webPages/file2.html HTTP/1.1"		
		echo "Host: $HOST:$PORT"
		echo ""
		sleep 1
	} | telnet $HOST $PORT)
	printf "%s\n" "$response"
	printf "___________________________________________________\n\n"
	response_status=$(echo "$response" | grep "HTTP" | awk '{print $2}')
	expected_status="200"
	if [[ $response_status ==  $expected_status ]]; then
        printf "${GREEN}${BOLD}SUCCESS 🥳${NC}"
    else
        printf "${RED}${BOLD}FAILURE 😭${NC}"
    fi
}

function gettest2 {
	printf "====================================================\n\n\n"
	printf "${BLUE}${BOLD}${UNDERLINE}* TEST 2: DISPLAY AN INDEX\n\n\n${NC}"
	printf "${BOLD}${UNDERLINE}REQUEST\n${NC}"
	printf "___________________________________________________\n\n"
	printf "GET / HTTP/1.1\\\r\\\n	         \n"
	printf "Host: %s:%s\\\r\\\n\n" "$HOST" "$PORT"
	printf "\\\r\\\n                                             \n"
	printf "___________________________________________________\n\n\n"	
	printf "${BOLD}${UNDERLINE}RESPONSE\n${NC}"
	printf "___________________________________________________\n\n"
	response=$( {
		echo "GET / HTTP/1.1"		
		echo "Host: $HOST:$PORT"
		echo ""
		sleep 1
	} | telnet $HOST $PORT)
	printf "%s\n" "$response"
	printf "___________________________________________________\n\n"
	response_status=$(echo "$response" | grep "HTTP" | awk '{print $2}')
	expected_status="200"
	if [[ $response_status ==  $expected_status ]]; then
        printf "${GREEN}${BOLD}SUCCESS 🥳${NC}"
    else
        printf "${RED}${BOLD}FAILURE 😭${NC}"
    fi
}

function gettest3 {
	printf "====================================================\n\n\n"
	printf "${BLUE}${BOLD}${UNDERLINE}* TEST 3: DISPLAY A LISTING\n\n\n${NC}"
	printf "${BOLD}${UNDERLINE}REQUEST\n${NC}"
	printf "___________________________________________________\n"
	printf "GET /webPages/ HTTP/1.1\\\r\\\n	         \n"
	printf "Host: %s:%s\\\r\\\n\n" "$HOST" "$PORT"
	printf "\\\r\\\n                                             \n"
	printf "___________________________________________________\n\n\n"	
	printf "${BOLD}${UNDERLINE}RESPONSE\n${NC}"
	printf "___________________________________________________\n\n"
	response=$( {
		echo "GET /webPages/ HTTP/1.1"		
		echo "Host: $HOST:$PORT"
		echo ""
		sleep 1
	} | telnet $HOST $PORT)
	printf "%s\n" "$response"
	printf "___________________________________________________\n\n\n"
	response_status=$(echo "$response" | grep "HTTP" | awk '{print $2}')
	expected_status="200"
	if [[ $response_status ==  $expected_status ]]; then
        printf "${GREEN}${BOLD}SUCCESS 🥳${NC}"
    else
        printf "${RED}${BOLD}FAILURE 😭${NC}"
    fi
}

function gettest4 {
	printf "====================================================\n\n\n"
	printf "${BLUE}${BOLD}${UNDERLINE}* TEST 4: USE CGI WITH QUERY STRING\n\n\n${NC}"
	printf "${BOLD}${UNDERLINE}REQUEST\n${NC}"
	printf "___________________________________________________\n\n"
	printf "GET /cgi-bin/scriptCgi.py?name=Ya&comment=Hello HTTP/1.1\\\r\\\n\n"
	printf "Host: %s:%s\\\r\\\n\n" "$HOST" "$PORT"
	printf "\\\r\\\n                                             \n"
	printf "___________________________________________________\n\n\n"	
	printf "${BOLD}${UNDERLINE}RESPONSE\n${NC}"
	printf "___________________________________________________\n\n"
	response=$( {
		echo "GET /cgi-bin/scriptCgi.py?name=Ya&comment=Hello HTTP/1.1"		
		echo "Host: $HOST:$PORT"
		echo ""
		sleep 1
	} | telnet $HOST $PORT)
	printf "%s\n" "$response"
	printf "___________________________________________________\n\n\n"
	response_status=$(echo "$response" | grep "HTTP" | awk '{print $2}')
	expected_status="200"
	if [[ $response_status ==  $expected_status ]]; then
        printf "${GREEN}${BOLD}SUCCESS 🥳${NC}"
    else
        printf "${RED}${BOLD}FAILURE 😭${NC}"
    fi
}

gettests=(
	gettest1
	gettest2
	gettest3
	gettest4
)

function run_gettests {
    for test in "${gettests[@]}"; do
        $test
		printf "\n\n\n====================================================\n"
        printf "${BOLD}${HIGHLIGHT}Press Enter to proceed to the next test...          ${NC}"
        read
    done
}

run_gettests


function deletetest1 {
	printf "\n\n\n            ${YELLOW}_______________________________\n"
	printf "            |                             |\n"
	printf "            |    TESTING METHOD: DELETE   |\n"
	printf "            |_____________________________|\n\n\n${NC}"
	printf "====================================================\n\n\n"
	printf "${BLUE}${BOLD}${UNDERLINE}* TEST 1: DELETE A FILE\n\n\n${NC}"
	printf "${BOLD}${UNDERLINE}REQUEST\n${NC}"
	printf "___________________________________________________\n\n"
	printf "DELETE /delete/okok HTTP/1.1\\\r\\\n	 \n"
	printf "Host: %s:%s\\\r\\\n\n" "$HOST" "$PORT"
	printf "\\\r\\\n                                             \n"
	printf "___________________________________________________${NC}\n\n\n"	
	printf "${BOLD}${UNDERLINE}RESPONSE\n${NC}"
	printf "___________________________________________________\n\n"
	response=$( {
		echo "DELETE /delete/okok HTTP/1.1"		
		echo "Host: $HOST:$PORT"
		echo ""
		sleep 1
	} | telnet $HOST $PORT)
	printf "%s\n" "$response"
	printf "___________________________________________________\n\n"
	response_status=$(echo "$response" | grep "HTTP" | awk '{print $2}')
	expected_status="200"
	if [[ $response_status ==  $expected_status ]]; then
        printf "${GREEN}${BOLD}SUCCESS 🥳${NC}"
    else
        printf "${RED}${BOLD}FAILURE 😭${NC}"
    fi
}

function deletetest2 {
	printf "====================================================\n\n\n"
	printf "${BLUE}${BOLD}${UNDERLINE}* TEST 2: DELETE A DIRECTORY\n\n\n${NC}"
	printf "${BOLD}${UNDERLINE}REQUEST\n${NC}"
	printf "___________________________________________________\n\n"
	printf "DELETE /delete/dir/ HTTP/1.1\\\r\\\n	         \n"
	printf "Host: %s:%s\\\r\\\n\n" "$HOST" "$PORT"
	printf "\\\r\\\n                                             \n"
	printf "___________________________________________________\n\n\n"	
	printf "${BOLD}${UNDERLINE}RESPONSE\n${NC}"
	printf "___________________________________________________\n\n"
	response=$( {
		echo "DELETE /delete/dir/ HTTP/1.1"		
		echo "Host: $HOST:$PORT"
		echo ""
		sleep 1
	} | telnet $HOST $PORT)
	printf "%s\n" "$response"
	printf "___________________________________________________\n\n"
	response_status=$(echo "$response" | grep "HTTP" | awk '{print $2}')
	expected_status="200"
	if [[ $response_status ==  $expected_status ]]; then
        printf "${GREEN}${BOLD}SUCCESS 🥳${NC}"
    else
        printf "${RED}${BOLD}FAILURE 😭${NC}"
    fi
}

deletetests=(
	deletetest1
	deletetest2
)

function run_deletetests {
    for test in "${deletetests[@]}"; do
        $test
		printf "\n\n\n====================================================\n"
        printf "${BOLD}${HIGHLIGHT}Press Enter to proceed to the next test...          ${NC}"
        read
    done
}

run_deletetests






function posttest1 {
	printf "\n\n\n            ${YELLOW}_____________________________\n"
	printf "            |                           |\n"
	printf "            |    TESTING METHOD: POST   |\n"
	printf "            |___________________________|\n\n\n${NC}"
	printf "====================================================\n\n\n"
	printf "${BLUE}${BOLD}${UNDERLINE}* TEST 1: SUBMIT A COMMENT\n\n\n${NC}"
	printf "${BOLD}${UNDERLINE}REQUEST\n${NC}"
	printf "___________________________________________________\n\n"
	printf "POST /cgi-bin/scriptCgi.py HTTP/1.1\\\r\\\n	 \n"
	printf "Host: %s:%s\\\r\\\n\n" "$HOST" "$PORT"
	printf "Content-Type: application/x-www-form-urlencoded\\\r\\\n\n"
	printf "Content-Length: 35\\\r\\\n\n"
	printf "\\\r\\\n                                             \n"
	printf "name=Joann&comment=HI+TOUT+LE+MONDE                  \n"
	printf "___________________________________________________${NC}\n\n\n"	
	printf "${BOLD}${UNDERLINE}RESPONSE\n${NC}"
	printf "___________________________________________________\n\n"
	response=$( {
		echo "POST /cgi-bin/scriptCgi.py HTTP/1.1"		
		echo "Host: $HOST:$PORT"
		echo "Content-Type: application/x-www-form-urlencoded"
		echo "Content-Length: 35"
		echo ""
		echo -n "name=Joann&comment=HI+TOUT+LE+MONDE"
		sleep 1
	} | telnet $HOST $PORT)
	printf "%s\n" "$response"
	printf "___________________________________________________\n\n"
	response_status=$(echo "$response" | grep "HTTP" | awk '{print $2}')
	expected_status="200"
	if [[ $response_status ==  $expected_status ]]; then
        printf "${GREEN}${BOLD}SUCCESS 🥳${NC}"
    else
        printf "${RED}${BOLD}FAILURE 😭${NC}"
    fi
}

function posttest2 {
	printf "====================================================\n\n\n"
	printf "${BLUE}${BOLD}${UNDERLINE}* TEST 2: UPLOAD A FILE\n\n\n${NC}"
	printf "${BOLD}${UNDERLINE}REQUEST\n${NC}"
	printf "___________________________________________________\n\n"
	printf "GET / HTTP/1.1\\\r\\\n	         \n"
	printf "Host: %s:%s\\\r\\\n\n" "$HOST" "$PORT"
	printf "\\\r\\\n                                             \n"
	printf "___________________________________________________\n\n\n"	
	printf "${BOLD}${UNDERLINE}RESPONSE\n${NC}"
	printf "___________________________________________________\n\n"
	response=$( {
		echo "GET / HTTP/1.1"		
		echo "Host: $HOST:$PORT"
		echo ""
		sleep 1
	} | telnet $HOST $PORT)
	printf "%s\n" "$response"
	printf "___________________________________________________\n\n"
	response_status=$(echo "$response" | grep "HTTP" | awk '{print $2}')
	expected_status="200"
	if [[ $response_status ==  $expected_status ]]; then
        printf "${GREEN}${BOLD}SUCCESS 🥳${NC}"
    else
        printf "${RED}${BOLD}FAILURE 😭${NC}"
    fi
}

posttests=(
	posttest1
)

function run_posttests {
    for test in "${posttests[@]}"; do
        $test
		printf "\n\n\n====================================================\n"
        printf "${BOLD}${HIGHLIGHT}Press Enter to proceed to the next test...          ${NC}"
        read
    done
}

run_posttests