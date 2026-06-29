# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    07_basic_cgi.sh                                    :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/06/23 12:36:53 by jweber            #+#    #+#              #
#    Updated: 2026/06/23 12:51:17 by jweber           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

rm -rf config_file.json
rm -rf $HOME/goinfre/tmp/
rm -rf *.log

echo "TEST 7: basic cgi execution with 'cgi' field in configuration file"
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
IFS=$OLD_IFS

mkdir -p $HOME/goinfre/tmp/a
echo '#!/bin/bash' > $HOME/goinfre/tmp/a/coucou.sh
echo "echo -ne 'content-type:text/html\r\n'" >> $HOME/goinfre/tmp/a/coucou.sh
echo "echo -ne '\r\n'" >> $HOME/goinfre/tmp/a/coucou.sh
echo "echo -ne 'Hello, World!\r\n'" >> $HOME/goinfre/tmp/a/coucou.sh
chmod +111 $HOME/goinfre/tmp/a/coucou.sh

../webserv config_file.json >/dev/null 2>/dev/null &
WEBSERV_PID=$!

echo -ne \
"HTTP/1.1 200 OK\r\n"\
"content-type: text/html\r\n"\
"\r\n"\
"Hello, World!\r\n" > expected.log

REQ_1="GET /coucou.sh HTTP/1.1\r\n\r\n"
echo -ne $REQ_1 | stdbuf -oL nc localhost 4343 > log_req.log
sed --in-place '/Date/d' log_req.log # delete date line to use diff after
sed --in-place '/Set-Cookie/d' log_req.log # delete date line to use diff after

ERROR=0
MSG=""
DIFF_A=$(diff expected.log log_req.log)
DIFF_A_ERR=$?
if [ $DIFF_A_ERR -ne 0 ] ; then
	MSG_EXPECT=$(cat -e expected.log)
	MSG_GET=$(cat -e log_req.log)
	MSG+="\n\nfollowing REQUEST failed:\n~~~~~~~~~~~~~~\n'$REQ_1'\n~~~~~~~~~~~~~~\n"
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
#rm -rf config_file.json
#rm -rf $HOME/goinfre/tmp/
#rm -rf *.log
echo
echo

