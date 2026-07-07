
# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    03_no_methods_allowed.sh                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/06/23 10:31:25 by jweber            #+#    #+#              #
#    Updated: 2026/06/23 10:31:58 by jweber           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

rm -rf config_file.json
rm -rf $HOME/goinfre/tmp/
rm -rf *.log

echo "TEST 6: basic index, with directory listing on and off"
OLD_IFS=$IFS
IFS=""
CONFIG_FILE="\"host\":
[
	{
	    \"listen\":4343,
	    \"name\": \"host_a\",
	    \"root\":\"$HOME/goinfre/\",
		\"location\":
		[
			{
				\"name\": \"/a\",
				\"alias\": \"$HOME/goinfre/tmp/a\",
				\"directory listing\": true,
				\"index\": \"index.html\"
			},
			{
				\"name\": \"/b\",
				\"alias\": \"$HOME/goinfre/tmp/b\",
				\"directory listing\": false,
				\"index\": \"index.html\"
			}
		]
	}
]"
echo $CONFIG_FILE > config_file.json
IFS=$OLD_IFS

mkdir -p $HOME/goinfre/tmp/a
echo -ne "in a" > $HOME/goinfre/tmp/a/index.html

mkdir -p $HOME/goinfre/tmp/b
echo -ne "in b" > $HOME/goinfre/tmp/b/index.html

../webserv config_file.json >/dev/null 2>/dev/null &
WEBSERV_PID=$!


REQ_1="GET /a HTTP/1.0\r\n\r\n"
echo -ne $REQ_1 | stdbuf -oL nc localhost 4343 > log_req_a.log
sed --in-place '/Date/d' log_req_a.log # delete date line to use diff after
sed --in-place '/Set-Cookie/d' log_req_a.log # delete date line to use diff after

echo -ne \
"HTTP/1.0 200 OK\r\n"\
"\r\n"\
"in a" > expected_a.log

REQ_2="GET /b/index.html HTTP/1.0\r\n\r\n"
echo -ne $REQ_2 | stdbuf -oL nc localhost 4343 > log_req_b.log
sed --in-place '/Date/d' log_req_b.log # delete date line to use diff after
sed --in-place '/Set-Cookie/d' log_req_b.log # delete date line to use diff after

echo -ne \
"HTTP/1.0 200 OK\r\n"\
"\r\n"\
"in b" > expected_b.log

ERROR=0
MSG=""
DIFF_A=$(diff expected_a.log log_req_a.log)
DIFF_A_ERR=$?
if [ $DIFF_A_ERR -ne 0 ] ; then
	MSG_EXPECT=$(cat -e expected_a.log)
	MSG_GET=$(cat -e log_req_a.log)
	MSG+="\n\nfollowing REQUEST failed:\n~~~~~~~~~~~~~~\n'$REQ_1'\n~~~~~~~~~~~~~~\n"
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


if [ $ERROR -ne 0 ]; then
	echo -ne "FAILED\n"
	echo -ne $MSG
	echo -ne "\nconfig file was :\n\n"
	cat config_file.json
else
	echo "SUCCESS";
fi

kill -INT $WEBSERV_PID
rm -rf config_file.json
rm -rf $HOME/goinfre/tmp/
rm -rf *.log
echo
echo

