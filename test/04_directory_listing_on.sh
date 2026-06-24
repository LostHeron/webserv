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

echo "TEST 4: directory listing on"
OLD_IFS=$IFS
IFS=""
CONFIG_FILE="\"host\":
[
	{
	    \"listen\":4343,
	    \"name\": \"host_a\",
	    \"root\":\"$HOME/goinfre/tmp/a\",
		\"directory listing\": true
	}
]"
echo $CONFIG_FILE > config_file.json
IFS=$OLD_IFS

mkdir -p $HOME/goinfre/tmp/a
echo -ne "in a" > $HOME/goinfre/tmp/a/index.html

../webserv config_file.json >/dev/null 2>/dev/null &
WEBSERV_PID=$!

echo -ne \
"HTTP/1.1 200 OK\r\n"\
"\r\n"\
"<html>\n"\
"<head><title>Index of /</title></head>\n"\
"<h1>Index of /</h1>\n"\
"<body>\n"\
"<ul>\n"\
"<li><a href=\"../\">../</a></li>\n"\
"<li><a href=\"./\">./</a></li>\n"\
"<li><a href=\"index.html\">index.html</a></li>\n"\
"\n"\
"</ul>\n"\
"\n"\
"</body>\n"\
"</html>\n" > expected.log


REQ_1="GET / HTTP/1.1\r\n\r\n"
echo -ne $REQ_1 | nc localhost 4343 > log_req.log
sed --in-place '/Date/d' log_req.log # delete date line to use diff after

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

