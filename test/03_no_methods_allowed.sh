# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    03_no_methods_allowed.sh                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/06/23 10:31:25 by jweber            #+#    #+#              #
#    Updated: 2026/06/26 17:58:09 by jweber           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

rm -rf config_file.json
rm -rf $HOME/goinfre/tmp/
rm -rf *.log

echo "TEST 3: no method allowed"

OLD_IFS=$IFS
IFS=""
CONFIG_FILE="\"host\":
[
	{
		\"listen\":4343,
		\"name\": \"host_a\",
		\"root\":\"$HOME/goinfre/tmp/a\",
		\"request\":[\"\"]
	}
]"

echo $CONFIG_FILE > config_file.json

../webserv config_file.json >/dev/null 2>/dev/null &
WEBSERV_PID=$!

mkdir -p $HOME/goinfre/tmp/a
echo -ne "in a" > $HOME/goinfre/tmp/a/index.html

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


########### test_1 with GET method

EXPECTED_VAR="HTTP/1.1 405 Method Not Allowed\r\n"

REQUEST_VAR="GET /index.html HTTP/1.1\r\n\r\n"

tests $EXPECTED_VAR $REQUEST_VAR "1"

######  test 2 with POST method

EXPECTED_VAR="HTTP/1.1 405 Method Not Allowed\r\n"

REQUEST_VAR="POST /index.html HTTP/1.1\r\n\r\n"

tests $EXPECTED_VAR $REQUEST_VAR "2"

######  test 3 with DELETE method

EXPECTED_VAR="HTTP/1.1 405 Method Not Allowed\r\n"

REQUEST_VAR="POST /index.html HTTP/1.1\r\n\r\n"

tests $EXPECTED_VAR $REQUEST_VAR "3"

######  test 3 with NOT_KNOWN method

EXPECTED_VAR="HTTP/1.1 405 Method Not Allowed\r\n"

REQUEST_VAR="NOTKNOWN /index.html HTTP/1.1\r\n\r\n"

tests $EXPECTED_VAR $REQUEST_VAR "4"

###### display results 

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

