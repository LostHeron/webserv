# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    12_test_delete_request.sh                          :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jweber <jweber@student.42Lyon.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/06/26 14:51:22 by jweber            #+#    #+#              #
#    Updated: 2026/06/26 17:59:39 by jweber           ###   ########.fr        #
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
		\"root\":\"$HOME/goinfre/tmp/\"
	}
]"

echo $CONFIG_FILE > config_file.json

../webserv config_file.json >/dev/null 2>/dev/null &
WEBSERV_PID=$!

# initialisation 
ERROR=0
MSG=""

################# TEST 4  delete of / which is a directory


rm -rf $HOME/goinfre/tmp/
mkdir -p $HOME/goinfre/tmp/a

EXPECTED_FILE=expected_a.log
RESULT_FILE=result_a.log

echo -ne \
"HTTP/1.0 403 Forbidden\r\n" > $EXPECTED_FILE

REQ="DELETE /a/ HTTP/1.0\r\n"\
"\r\n"
# the 'stdbuf -oL' flushes the buffer into the file,
# without it, we had some issue where sometimes log_req_a.log
# was empty
echo -ne $REQ | stdbuf -o0 nc localhost 4343 > $RESULT_FILE
head -1 $RESULT_FILE > tmp.log # keep only first line
cat tmp.log > $RESULT_FILE


EXPECTED_TREE=expected_tree_a.log
RESULT_TREE=result_tree_a.log

echo -ne \
"└── a\n"\
"\n"\
"1 directory, 0 files\n"> $EXPECTED_TREE

tree $HOME/goinfre/tmp > $RESULT_TREE
tail -n +2 $RESULT_TREE > tmp.log # delete first line
cat tmp.log > $RESULT_TREE

DIFF=$(diff $EXPECTED_FILE $RESULT_FILE)
DIFF_ERR=$?
DIFF=$(diff $EXPECTED_TREE $RESULT_TREE)
DIFF_ERR_TREE=$?
if [ $DIFF_ERR -ne 0 ] || [ $DIFF_ERR_TREE -ne 0 ]; then
	MSG_EXPECT=$(cat -e $EXPECTED_FILE)
	MSG_RESULT=$(cat -e $RESULT_FILE)
	MSG+="\n\nfollowing REQUEST failed:\n~~~~~~~~~~~~~~\n'$REQ'\n~~~~~~~~~~~~~~\n"
	MSG+="\n\nexpected:\n"
	MSG+=$MSG_EXPECT
	MSG+="\n\nresult:\n";
	MSG+=$MSG_RESULT
	MSG+="\n\nexpected_tree:\n"
	MSG+=$(cat $EXPECTED_TREE)
	MSG+="\n\nresult_tree:\n";
	MSG+=$(cat $RESULT_TREE)
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
