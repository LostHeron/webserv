# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    13_test_put.sh                                     :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/07/09 12:23:29 by jweber            #+#    #+#              #
#    Updated: 2026/07/09 12:40:22 by jweber           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

rm -rf config_file.json
rm -rf $HOME/goinfre/tmp/
rm -rf *.log

echo "test 13: put request"

OLD_IFS=$IFS
IFS=""
CONFIG_FILE="\"host\":
[
	{
		\"name\": \"host_a\",
		\"listen\":[4343],
		\"request\":[\"PUT\"],
		\"root\":\"$HOME/goinfre/tmp/a\",
		\"extension\":[\".sh\"]
	}
]"

echo $CONFIG_FILE > config_file.json

mkdir -p $HOME/goinfre/tmp/a

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

################ TEST 1 creating a file 

EXPECTED_VAR="HTTP/1.0 201 Created\r\n"

REQUEST_VAR="PUT /test.txt HTTP/1.0\r\n"\
"content-length:10\r\n"\
"\r\n"\
"coucou toi"\

tests_first_line $EXPECTED_VAR $REQUEST_VAR "a"


################ TEST 2  file already existing

EXPECTED_VAR="HTTP/1.0 204 No Content\r\n"

REQUEST_VAR="PUT /test.txt HTTP/1.0\r\n"\
"content-length:10\r\n"\
"\r\n"\
"coucou lui"\

tests_first_line $EXPECTED_VAR $REQUEST_VAR "b"


################ TEST 3 two request trying to edit same file
# at the same time


#python3 -c \
#"import time
#print(\"PUT /test.txt HTTP/1.0\\r\\n\", flush=True, end=\"\")
#print(\"content-length:10\r\n\", flush=True, end=\"\")
#print(\"\r\n\", flush=True, end=\"\")
#time.sleep(1)
#print(\"coucoucouccoucou\r\n\", flush=True, end=\"\")" | nc localhost 4343


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
