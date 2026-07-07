#!/bin/bash

echo "content-type: text/html"
COLOR="ffffff"
if [[ $REQUEST_METHOD == "GET" ]]; then
	if [[ $HTTP_COOKIE != "" ]]; then
		eval $HTTP_COOKIE
		COOKIE_ID=$id
		COLOR=$(sqlite3 colors.db "select color from colors where id = \"$COOKIE_ID\";")
		# check color is ok 
		if [[ COLOR == "" ]]; then
			COLOR="ffffff"
		fi
	fi
fi

if [[ $REQUEST_METHOD == "POST" ]];then
	read NEW_COLOR_DATA
	eval $NEW_COLOR_DATA
	COLOR=$color
	if [[ $HTTP_COOKIE == "" ]];then
		# here generate a cookie
		if [[ $COLOR == "" ]];then
			COLOR="ffffff"
		fi

		ID=$(date +%s);
		SET_COOKIE="id=$ID; Max-Age=60"
		echo "Set-Cookie: $SET_COOKIE"
		# maybe a check to be sure value does not already
		# exist in db"
		sqlite3 colors.db "insert into colors values(\"$ID\", \"$COLOR\");"
	else
		# here need to change in the table the row corresponding
		# to an id
		eval $HTTP_COOKIE
		COOKIE_ID=$id

		if [[ $COLOR == "" ]];then
			COLOR=$(sqlite3 colors.db "select color from colors where id = \"$COOKIE_ID\";")
		else
			sqlite3 colors.db "update colors set color=\"$COLOR\" where id=\"$COOKIE_ID\";"
		fi
	fi
fi

echo
echo -ne "<!DOCTYPE html>\n"\
"<html>\n"\
"<body style=\"background-color:#$COLOR;\">\n"\
"\n"\
"<h1>This is a heading</h1>\n"\
"<p>This is a paragraph.</p>\n"\
"<p>HTTP_COOKIE=\"$HTTP_COOKIE\"</p>\n"\
"<p>HTTP_COOKIE_ID=\"$COOKIE_ID\"</p>\n"\
"\n"\
"    <form action=\"/test_cookie.sh\" method=\"post\" enctype=\"application/x-www-form-urlencoded\" name=\"myform\">\n"\
"<select name=\"color\" id=\"pet-select\">\n"\
"  <option value=\"\">--Please choose an option--</option>\n"\
"  <option value=\"ffffff\">white</option>\n"\
"  <option value=\"00ff00\">green</option>\n"\
"  <option value=\"0000ff\">blue</option>\n"\
"  <option value=\"ff0000\">red</option>\n"\
"</select>\n"\
"        <input type=\"submit\" value=\"change color\">\n"\
"    </form>\n"\
"\n"\
"</body>\n"\
"</html>\n"
