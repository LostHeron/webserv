# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    07_03_same_as_07_02_without_cgi.sh                 :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/06/30 16:01:43 by jweber            #+#    #+#              #
#    Updated: 2026/06/30 16:08:45 by jweber           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

rm -rf config_file.json
rm -rf $HOME/goinfre/tmp/
rm -rf *.log

echo "TEST 7 tris: same as 07 02 but without extension"

OLD_IFS=$IFS
IFS=""
CONFIG_FILE="\"host\":
[
	{
		\"name\": \"host_a\",
		\"listen\":[4343],
		\"root\":\"$HOME/goinfre/tmp/a\"
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

	echo -ne $REQUEST | stdbuf -o0 nc localhost 4343 > $RESULT_FILE
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

	echo -ne $REQUEST | stdbuf -o0 nc localhost 4343 > $RESULT_FILE
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

EXPECTED_VAR='HTTP/1.0 200 OK\r\n'\
'\r\n'\
'#!/bin/bash\n'\
'echo "content-type:text/html"\n'\
'echo \n'\
'echo "Hello, World!"\n'\
'echo "PATH_INFO=$PATH_INFO"\n'

REQUEST_VAR="GET /coucou.sh HTTP/1.0\r\n\r\n"

tests $EXPECTED_VAR $REQUEST_VAR "a"

################ TEST 2 with no path info

EXPECTED_VAR="HTTP/1.0 403 Forbidden\r\n"\

REQUEST_VAR="GET /coucou.sh/index.html HTTP/1.0\r\n\r\n"

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
