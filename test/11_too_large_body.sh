# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    11_too_large_body.sh                               :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/06/26 10:30:53 by jweber            #+#    #+#              #
#    Updated: 2026/06/26 11:04:36 by jweber           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

echo "TEST 11: too large body"

OLD_IFS=$IFS
IFS=""
CONFIG_FILE="\"host\":
[
	{
		\"name\": \"host_a\",
		\"listen\":[4343],
		\"root\":\"$HOME/goinfre/tmp/a\"
	},
	{
		\"name\": \"host_b\",
		\"listen\":[4343],
		\"max body\":1000,
		\"root\":\"$HOME/goinfre/tmp/b\"
	}
]"

echo $CONFIG_FILE > config_file.json
IFS=$OLD_IFS

mkdir -p $HOME/goinfre/tmp/a
mkdir -p $HOME/goinfre/tmp/b

echo -ne "in a" > $HOME/goinfre/tmp/a/index.html
echo -ne "in b" > $HOME/goinfre/tmp/b/index.html

../webserv config_file.json >/dev/null 2>/dev/null &
WEBSERV_PID=$!

# initialisation 
ERROR=0
MSG=""

################ TEST 1
EXPECTED_FILE=expected_a.log
RESULT_FILE=result_a.log

echo -ne \
"HTTP/1.1 200 OK\r\n"\
"\r\n"\
"in a" > $EXPECTED_FILE

REQ="GET /index.html HTTP/1.0\r\n"\
"host:host_a\r\n"\
"content-length:1001\r\n"\
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

################ TEST 2
EXPECTED_FILE=expected_b.log
RESULT_FILE=result_b.log

echo -ne \
"HTTP/1.1 413 Payload Too Large\r\n"\
"\r\n"\
"<html>\n"\
"<head><title>413 Payload Too Large</title></head>\n"\
"<body>\n"\
"<h1>413 Payload Too Large</h1>\n"\
"\n"\
"</body>\n"\
"</html>\n" > $EXPECTED_FILE

REQ="GET /index.html HTTP/1.0\r\n"\
"host:host_b\r\n"\
"content-length:1001\r\n"\
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
