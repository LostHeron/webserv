# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    02_location_basic_test.sh                          :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/06/09 17:05:57 by jweber            #+#    #+#              #
#    Updated: 2026/06/09 18:12:36 by jweber           ###   ########.fr        #
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
IFS=$OLD_IFS

mkdir -p $HOME/goinfre/tmp/a
echo -ne "in a" > $HOME/goinfre/tmp/a/index_a.html
mkdir -p $HOME/goinfre/tmp/b
echo -ne "in b" > $HOME/goinfre/tmp/b/index_b.html

../webserv config_file.json >/dev/null 2>/dev/null &
WEBSERV_PID=$!

echo -ne "HTTP/1.1 200 OK\r\n\r\nin a" > expected_a.log
REQ_1="GET /index_a.html HTTP/1.1\r\nhost:host_a\r\n\r\n"
echo -ne $REQ_1 | nc localhost 4343 > log_req_a.log
sed --in-place '/Date/d' log_req_a.log
sed --in-place '/Set-Cookie/d' log_req_a.log # delete date line to use diff after

#echo "sending following request"
#echo -n "GET /index_b.html HTTP/1.1\r\nhost:host_b\r\n\r\n"
echo -ne "HTTP/1.1 200 OK\r\n\r\nin b" > expected_b.log
REQ_2="GET /images/index_b.html HTTP/1.1\r\nhost:host_b\r\n\r\n" 
echo -ne $REQ_2 | nc localhost 4343 > log_req_b.log
sed --in-place '/Date/d' log_req_b.log
sed --in-place '/Set-Cookie/d' log_req_b.log # delete date line to use diff after

ERROR=0
MSG=""
DIFF_A=$(diff expected_a.log log_req_a.log)
DIFF_A_ERR=$?
if [ $DIFF_A_ERR -ne 0 ] ; then
	MSG_EXPECT=$(cat -e expected_a.log)
	MSG_GET=$(cat -e log_req_a.log)
	MSG+="\n\nfollowing REQUEST failed:\n~~~~~~~~~~~~~~\n'$REQ_2'\n~~~~~~~~~~~~~~\n"
	MSG+="expected:\n"
	MSG+=$MSG_EXPECT
	MSG+="\nget:\n";
	MSG+=$MSG_GET
	MSG+="\n"
	ERROR+=1
fi

DIFF_B=$(diff expected_b.log log_req_b.log)
DIFF_B_ERR=$?
if [ $DIFF_B_ERR -ne 0 ] ; then
	MSG_EXPECT=$(cat -e expected_b.log)
	MSG_GET=$(cat -e log_req_b.log)
	MSG+="\n\nfollowing REQUEST failed:\n~~~~~~~~~~~~~~\n'$REQ_2'\n~~~~~~~~~~~~~~\n"
	MSG+="expected:\n"
	MSG+=$MSG_EXPECT
	MSG+="\nget:\n";
	MSG+=$MSG_GET
	MSG+="\n"
	ERROR+=1
fi

#echo "coucou~~~~~~~~~~~~~~~~~~~~~~~~"
#echo "MSG_EXCPECT = $MSG_EXPECT"
#echo "coucou~~~~~~~~~~~~~~~~~~~~~~~~"
#echo "MSG_GET = $MSG_GET"
#echo "coucou~~~~~~~~~~~~~~~~~~~~~~~~"
#echo "MSG = $MSG"
#echo "coucou~~~~~~~~~~~~~~~~~~~~~~~~"

if [ $ERROR -ne 0 ]; then
	echo -ne "FAILED basic location test\n"
	echo -ne $MSG
	echo -ne "\nconfig file was :\n\n"
	cat config_file.json
else
	echo "SUCCESS";
fi

kill -INT $WEBSERV_PID
#rm -rf config_file.json
#rm -rf $HOME/goinfre/tmp/
#rm -r *.log
echo
echo

