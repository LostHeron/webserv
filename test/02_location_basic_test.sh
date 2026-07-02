# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    02_location_basic_test.sh                          :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/06/09 17:05:57 by jweber            #+#    #+#              #
#    Updated: 2026/06/26 17:58:19 by jweber           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

rm -rf config_file.json
rm -rf $HOME/goinfre/tmp/
rm -rf *.log

echo "TEST 2: Basic location"
OLD_IFS=$IFS
IFS=""
CONFIG_FILE="\"host\":
[
	{
	    \"listen\":4343,
	    \"name\": \"host_a\",
	    \"root\":\"$HOME/goinfre/tmp/a\",
		\"request\":[\"GET\",\"POST\"],
		\"location\":
	    [
	    	{
	    	\"name\":\"/images\",
	    	\"alias\":\"$HOME/goinfre/tmp/b/\"
	    	}
	    ]
	}
]"
echo $CONFIG_FILE > config_file.json

mkdir -p $HOME/goinfre/tmp/a
echo -ne "in a" > $HOME/goinfre/tmp/a/index_a.html
mkdir -p $HOME/goinfre/tmp/b
echo -ne "in b" > $HOME/goinfre/tmp/b/index_b.html

../webserv config_file.json >/dev/null 2>/dev/null &
WEBSERV_PID=$!

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

############### test 1

EXPECTED_VAR="HTTP/1.1 200 OK\r\n\r\nin a"
REQUEST_VAR="GET /index_a.html HTTP/1.1\r\nhost:host_a\r\n\r\n"

tests $EXPECTED_VAR $REQUEST_VAR "a"

################# test 2

EXPECTED_VAR="HTTP/1.1 200 OK\r\n\r\nin b"
REQUEST_VAR="GET /images/index_b.html HTTP/1.1\r\nhost:host_b\r\n\r\n" 

tests $EXPECTED_VAR $REQUEST_VAR "b"

############### test 1

EXPECTED_VAR="HTTP/1.1 200 OK\r\n\r\nin a"
REQUEST_VAR="POST /index_a.html HTTP/1.1\r\nhost:host_a\r\n\r\n"

tests $EXPECTED_VAR $REQUEST_VAR "a"

################# test 2

EXPECTED_VAR="HTTP/1.1 200 OK\r\n\r\nin b"
REQUEST_VAR="POST /images/index_b.html HTTP/1.1\r\nhost:host_b\r\n\r\n" 

tests $EXPECTED_VAR $REQUEST_VAR "b"

############# display result

if [ $ERROR -ne 0 ]; then
	echo -ne "FAILED basic location test\n"
	echo -ne $MSG
	echo -ne "\nconfig file was :\n\n"
	cat config_file.json
else
	echo "SUCCESS";
fi

kill -INT $WEBSERV_PID
IFS=$OLD_IFS
#rm -rf config_file.json
#rm -rf $HOME/goinfre/tmp/
#rm -r *.log
echo
echo

