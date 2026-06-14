########################## TEST 1 ##################################
echo "TEST 1: Host selection"
OLD_IFS=$IFS
IFS=""
CONFIG_FILE="\"host\":[
{
    \"listen\":4343,
    \"name\": \"host_a\",
    \"root\":\"$HOME/goinfre/tmp/a\"
},

{
    \"listen\":4343,
    \"name\": \"host_b\",
    \"root\":\"$HOME/goinfre/tmp/b\"
}]"
#echo $CONFIG_FILE
echo $CONFIG_FILE > config_file.json
IFS=$OLD_IFS

mkdir -p $HOME/goinfre/tmp/a
echo "in a" > $HOME/goinfre/tmp/a/index_a.html
mkdir -p $HOME/goinfre/tmp/b
echo "in b" > $HOME/goinfre/tmp/b/index_b.html

../webserv config_file.json >/dev/null 2>/dev/null &
WEBSERV_PID=$!
#echo "weberserv pid = $WEBSERV_PID"

#echo "sending following request"
#echo -n "GET /index_a.html HTTP/1.1\r\nhost:host_a\r\n\r\n"
echo -ne "GET /index_a.html HTTP/1.1\r\nhost:host_a\r\n\r\n" | nc localhost 4343 > log_req_a.log

#echo "sending following request"
#echo -n "GET /index_b.html HTTP/1.1\r\nhost:host_b\r\n\r\n"
echo -ne "GET /index_b.html HTTP/1.1\r\nhost:host_b\r\n\r\n" | nc localhost 4343 > log_req_b.log

LAST_LINE_A=$(tail -1 log_req_a.log)
LAST_LINE_B=$(tail -1 log_req_b.log)

if [ "$LAST_LINE_A" != "in a" ] || [ "$LAST_LINE_B" != "in b" ]; then
	echo "FAILED basic host selection test";
else
	echo "SUCCESS";
fi

kill -INT $WEBSERV_PID
#rm -rf config_file.json
rm -rf $HOME/goinfre/tmp/
echo
echo

