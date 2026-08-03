# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    07_01_cgi_with_cgi_field.sh                        :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/06/30 16:17:45 by jweber            #+#    #+#              #
#    Updated: 2026/06/30 16:25:50 by jweber           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

rm -rf config_file.json
rm -rf $HOME/goinfre/tmp/
rm -rf *.log

echo "TEST 7: try cgi with 'cgi' field in configuration file"

OLD_IFS=$IFS
IFS=""
CONFIG_FILE="\"host\":
[
	{
		\"listen\":4343,
		\"name\": \"host_a\",
		\"root\":\"$HOME/goinfre/tmp/a\",
		\"cgi\": true
	}
]"
echo $CONFIG_FILE > config_file.json

mkdir -p $HOME/goinfre/tmp/a

echo -ne \
'#!/bin/bash\n'\
'echo "content-type:text/html"\n'\
'echo \n'\
'echo "Hello, World!"\n'\
'echo "PATH_INFO=$PATH_INFO"\n' > $HOME/goinfre/tmp/a/coucou.sh
chmod +111 $HOME/goinfre/tmp/a/coucou.sh

../webserv config_file.json >/dev/null 2>/dev/null &
WEBSERV_PID=$!

# initialisation 
ERROR=0
MSG=""

function tests()
{
	EXPECTED=$1
	REQUEST=$2
	TEST_NUMBER=$3

	EXPECTED_FILE=expected_$TEST_NUMBER.log
	RESULT_FILE=result_$TEST_NUMBER.log

	echo -ne $EXPECTED > $EXPECTED_FILE

	stdbuf -o0 echo -ne $REQUEST | stdbuf -i0 -o0 nc localhost 4343 > $RESULT_FILE
	sed --in-place '/Date/d' $RESULT_FILE
	sed --in-place '/Set-Cookie/d' $RESULT_FILE # delete date line to use diff after

	DIFF_A=$(diff $RESULT_FILE $EXPECTED_FILE)
	DIFF_A_ERR=$?
	if [ $DIFF_A_ERR -ne 0 ] ; then
		MSG_EXPECT=$(cat -e $EXPECTED_FILE)
		MSG_GET=$(cat -e $RESULT_FILE)
		MSG+="\n\nfollowing REQUEST failed:\n~~~~~~~~~~~~~~\n'$REQUEST'\n~~~~~~~~~~~~~~\n"
		MSG+="expected:\n"
		MSG+=$MSG_EXPECT
		MSG+="\nget:\n";
		MSG+=$MSG_GET
		MSG+="\n"
		ERROR+=1
	fi
}

function tests_first_line()
{
	EXPECTED=$1
	REQUEST=$2
	TEST_NUMBER=$3

	EXPECTED_FILE=expected_$TEST_NUMBER.log
	RESULT_FILE=result_$TEST_NUMBER.log

	echo -ne $EXPECTED > $EXPECTED_FILE

	stdbuf -o0 echo -ne $REQUEST | stdbuf -i0 -o0 nc localhost 4343 > $RESULT_FILE
	head -1 $RESULT_FILE > tmp.log
	cat tmp.log > $RESULT_FILE

	DIFF_A=$(diff $RESULT_FILE $EXPECTED_FILE)
	DIFF_A_ERR=$?
	if [ $DIFF_A_ERR -ne 0 ] ; then
		MSG_EXPECT=$(cat -e $EXPECTED_FILE)
		MSG_GET=$(cat -e $RESULT_FILE)
		MSG+="\n\nfollowing REQUEST failed:\n~~~~~~~~~~~~~~\n'$REQUEST'\n~~~~~~~~~~~~~~\n"
		MSG+="expected:\n"
		MSG+=$MSG_EXPECT
		MSG+="\nget:\n";
		MSG+=$MSG_GET
		MSG+="\n"
		ERROR+=1
	fi
}


################ TEST 1 with no path info

EXPECTED_VAR="HTTP/1.0 200 OK\r\n"\
"content-type: text/html\r\n"\
"\r\n"\
"Hello, World!\n"\
"PATH_INFO=/\n"

REQUEST_VAR="GET /coucou.sh HTTP/1.0\r\n\r\n"

tests $EXPECTED_VAR $REQUEST_VAR "a"

################ TEST 2 with path info that should be /index.html

EXPECTED_VAR="HTTP/1.0 200 OK\r\n"\
"content-type: text/html\r\n"\
"\r\n"\
"Hello, World!\n"\
"PATH_INFO=/index.html\n"

REQUEST_VAR="GET /coucou.sh/index.html HTTP/1.0\r\n\r\n"

tests $EXPECTED_VAR $REQUEST_VAR "b"


################ TEST 3 with a path not found 

EXPECTED_VAR="HTTP/1.0 404 Not Found\r\n"

REQUEST_VAR="GET /none_existin_file.sh HTTP/1.0\r\n\r\n"

tests_first_line $EXPECTED_VAR $REQUEST_VAR "b"

################# RESULT + clear

if [ $ERROR -ne 0 ]; then
	echo -ne "FAILED\n"
	echo -ne $MSG
	echo -ne "\nconfig file was :\n\n"
	cat config_file.json
else
	echo "SUCCESS";
fi

IFS=$OLD_IFS
kill -INT $WEBSERV_PID
#rm -rf config_file.json
#rm -rf $HOME/goinfre/tmp/
#rm -rf *.log
echo
echo
