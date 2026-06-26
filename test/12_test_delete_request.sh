# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    12_test_delete_request.sh                          :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/06/26 14:51:22 by jweber            #+#    #+#              #
#    Updated: 2026/06/26 16:51:18 by jweber           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

rm -rf config_file.json
rm -rf $HOME/goinfre/tmp/
rm -rf *.log

echo "TEST 12: delete request"

OLD_IFS=$IFS
IFS=""
CONFIG_FILE="\"host\":
[
	{
		\"name\": \"host_a\",
		\"listen\":[4343],
		\"request\":[\"DELETE\"],
		\"root\":\"$HOME/goinfre/tmp/a\"
	}
]"

echo $CONFIG_FILE > config_file.json
IFS=$OLD_IFS

mkdir -p $HOME/goinfre/tmp/a

echo -ne "in a" > $HOME/goinfre/tmp/a/index_a.html
echo -ne "in b" > $HOME/goinfre/tmp/a/index_b.html
chmod 0 $HOME/goinfre/tmp/a/index_b.html

../webserv config_file.json >/dev/null 2>/dev/null &
WEBSERV_PID=$!

# initialisation 
ERROR=0
MSG=""

################ TEST 1  delete of index_a.html
EXPECTED_FILE=expected_a.log
RESULT_FILE=result_a.log

echo -ne \
"HTTP/1.1 204 No Content\r\n"\
"\r\n"\
"in a" > $EXPECTED_FILE

REQ="DELETE /index_a.html HTTP/1.0\r\n"\
"\r\n"
# the 'stdbuf -oL' flushes the buffer into the file,
# without it, we had some issue where sometimes log_req_a.log
# was empty
echo -ne $REQ | stdbuf -o0 nc localhost 4343 > $RESULT_FILE
sed --in-place '/Date/d' $RESULT_FILE # delete date line to use diff after
sed --in-place '/Set-Cookie/d' $RESULT_FILE # delete date line to use diff after

DIFF=$(diff $EXPECTED_FILE $RESULT_FILE)
DIFF_ERR=$?
if [ $DIFF_ERR -ne 0 ] ; then
	MSG_EXPECT=$(cat -e $EXPECTED_FILE)
	MSG_RESULT=$(cat -e $RESULT_FILE)
	MSG+="\n\nfollowing REQUEST failed:\n~~~~~~~~~~~~~~\n'$REQ'\n~~~~~~~~~~~~~~\n"
	MSG+="expected:\n"
	MSG+=$MSG_EXPECT
	MSG+="\nget:\n";
	MSG+=$MSG_RESULT
	MSG+="\n"
	ERROR+=1
fi

################ TEST 2  delete of index_b.html which is chmod 0
# and should then be forbidden
EXPECTED_FILE=expected_a.log
RESULT_FILE=result_a.log

echo -ne \
"HTTP/1.1 403 Forbidden\r\n" > $EXPECTED_FILE

REQ="DELETE /index_b.html HTTP/1.0\r\n"\
"\r\n"
# the 'stdbuf -oL' flushes the buffer into the file,
# without it, we had some issue where sometimes log_req_a.log
# was empty
echo -ne $REQ | stdbuf -o0 nc localhost 4343 > $RESULT_FILE
head -1 $RESULT_FILE > tmp_file.log
cat tmp_file.log > $RESULT_FILE

DIFF=$(diff $EXPECTED_FILE $RESULT_FILE)
DIFF_ERR=$?
if [ $DIFF_ERR -ne 0 ] ; then
	MSG_EXPECT=$(cat -e $EXPECTED_FILE)
	MSG_RESULT=$(cat -e $RESULT_FILE)
	MSG+="\n\nfollowing REQUEST failed:\n~~~~~~~~~~~~~~\n'$REQ'\n~~~~~~~~~~~~~~\n"
	MSG+="expected:\n"
	MSG+=$MSG_EXPECT
	MSG+="\nget:\n";
	MSG+=$MSG_RESULT
	MSG+="\n"
	ERROR+=1
fi

################ TEST 3  delete of index_not_existing.html
# and should then be not found
EXPECTED_FILE=expected_a.log
RESULT_FILE=result_a.log

echo -ne \
"HTTP/1.1 404 Not Found\r\n" > $EXPECTED_FILE

REQ="DELETE /index_not_existing.html HTTP/1.0\r\n"\
"\r\n"
# the 'stdbuf -oL' flushes the buffer into the file,
# without it, we had some issue where sometimes log_req_a.log
# was empty
echo -ne $REQ | stdbuf -o0 nc localhost 4343 > $RESULT_FILE
head -1 $RESULT_FILE > tmp_file.log
cat tmp_file.log > $RESULT_FILE

DIFF=$(diff $EXPECTED_FILE $RESULT_FILE)
DIFF_ERR=$?
if [ $DIFF_ERR -ne 0 ] ; then
	MSG_EXPECT=$(cat -e $EXPECTED_FILE)
	MSG_RESULT=$(cat -e $RESULT_FILE)
	MSG+="\n\nfollowing REQUEST failed:\n~~~~~~~~~~~~~~\n'$REQ'\n~~~~~~~~~~~~~~\n"
	MSG+="expected:\n"
	MSG+=$MSG_EXPECT
	MSG+="\nget:\n";
	MSG+=$MSG_RESULT
	MSG+="\n"
	ERROR+=1
fi


################ TEST 3  delete of / which is a directory
# and should then be forbidden
EXPECTED_FILE=expected_a.log
RESULT_FILE=result_a.log

echo -ne \
"HTTP/1.1 403 Forbidden\r\n" > $EXPECTED_FILE

REQ="DELETE / HTTP/1.0\r\n"\
"\r\n"
# the 'stdbuf -oL' flushes the buffer into the file,
# without it, we had some issue where sometimes log_req_a.log
# was empty
echo -ne $REQ | stdbuf -o0 nc localhost 4343 > $RESULT_FILE
head -1 $RESULT_FILE > tmp_file.log
cat tmp_file.log > $RESULT_FILE

DIFF=$(diff $EXPECTED_FILE $RESULT_FILE)
DIFF_ERR=$?
if [ $DIFF_ERR -ne 0 ] ; then
	MSG_EXPECT=$(cat -e $EXPECTED_FILE)
	MSG_RESULT=$(cat -e $RESULT_FILE)
	MSG+="\n\nfollowing REQUEST failed:\n~~~~~~~~~~~~~~\n'$REQ'\n~~~~~~~~~~~~~~\n"
	MSG+="expected:\n"
	MSG+=$MSG_EXPECT
	MSG+="\nget:\n";
	MSG+=$MSG_RESULT
	MSG+="\n"
	ERROR+=1
fi


################# RESULT + clear

if [ $ERROR -ne 0 ]; then
	echo -ne "FAILED\n"
	echo -ne $MSG
	echo -ne "\nconfig file was :\n\n"
	cat config_file.json
else
	echo "SUCCESS";
fi

kill -INT $WEBSERV_PID
#rm -rf config_file.json
#rm -rf $HOME/goinfre/tmp/
#rm -rf *.log
echo
echo
