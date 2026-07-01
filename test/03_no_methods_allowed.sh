# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    03_no_methods_allowed.sh                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cviel <cviel@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/06/23 10:31:25 by jweber            #+#    #+#              #
#    Updated: 2026/06/26 17:52:14 by cviel            ###   ########.fr        #
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
IFS=$OLD_IFS

mkdir -p $HOME/goinfre/tmp/a
echo -ne "in a" > $HOME/goinfre/tmp/a/index.html

../webserv config_file.json >/dev/null 2>/dev/null &
WEBSERV_PID=$!

echo -ne \
"HTTP/1.1 403 Forbidden\r\n"\
"\r\n"\
"<html>\n"\
"<head><title>403 Forbidden</title></head>\n"\
"<body>\n"\
"<h1>403 Forbidden</h1>\n"\
"\n"\
"</body>\n"\
"</html>\n" > expected.log

REQ_1="GET /index.html HTTP/1.1\r\n\r\n"
echo -ne $REQ_1 | nc localhost 4343 > log_req.log
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

