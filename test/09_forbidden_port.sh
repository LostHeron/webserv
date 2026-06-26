
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

echo "TEST 9: forbidden port"
OLD_IFS=$IFS
IFS=""
CONFIG_FILE="\"host\":
[
	{
		\"listen\":[20],
		\"name\": \"host_a\",
		\"root\":\"$HOME/goinfre/tmp/a\"
	}
]"
echo $CONFIG_FILE > config_file.json
IFS=$OLD_IFS

mkdir -p $HOME/goinfre/tmp/a

../webserv config_file.json >/dev/null 2> get.log
WEBSERV_PID=$!
echo $WEBSERV_PID
if [ $WEBSERV_PID ]; then
	kill $WEBSERV_PID
fi

tail -1 get.log > new_get.log

cat new_get.log > get.log

echo -ne "could not launch server\n" > expected.log

ERROR=0
MSG=""
DIFF_A=$(diff expected.log get.log)
DIFF_A_ERR=$?
if [ $DIFF_A_ERR -ne 0 ] ; then
	MSG_EXPECT=$(cat -e expected.log)
	MSG_GET=$(cat -e get.log)
	MSG+="\n\nfollowing REQUEST failed:\n~~~~~~~~~~~~~~\n'$REQ_1'\n~~~~~~~~~~~~~~\n"
	MSG+="expected:\n"
	MSG+=$MSG_EXPECT
	MSG+="\nget:\n";
	MSG+=$MSG_GET
	MSG+="\n"
	ERROR+=1
fi
#
#DIFF_B=$(diff expected_b.log log_req_b.log)
#DIFF_B_ERR=$?
#if [ $DIFF_B_ERR -ne 0 ] ; then
#	MSG_EXPECT=$(cat -e expected_b.log)
#	MSG_GET=$(cat -e log_req_b.log)
#	MSG+="\n\nfollowing REQUEST failed:\n~~~~~~~~~~~~~~\n'$REQ_2'\n~~~~~~~~~~~~~~\n"
#	MSG+="expected:\n"
#	MSG+=$MSG_EXPECT
#	MSG+="\nget:\n";
#	MSG+=$MSG_GET
#	MSG+="\n"
#	ERROR+=1
#fi
#
#
if [ $ERROR -ne 0 ]; then
	echo -ne "FAILED\n"
	echo -ne $MSG
	echo -ne "\nconfig file was :\n\n"
	cat config_file.json
else
	echo "SUCCESS";
fi
#
#kill -INT $WEBSERV_PID
#rm -rf config_file.json
#rm -rf $HOME/goinfre/tmp/
#rm -rf *.log
echo
echo

