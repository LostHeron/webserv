# to install the scrtip just run bash ./install.sh

install: tester cgi_tester YoupiBanane

tester:
	wget https://cdn.intra.42.fr/document/document/46302/tester
	chmod +111 tester

cgi_tester:
	wget https://cdn.intra.42.fr/document/document/46303/cgi_tester
	chmod +111 cgi_tester

YoupiBanane:
	rm -rf YoupiBanane
	mkdir YoupiBanane
	echo "youpi.bad_extension" > YoupiBanane/youpi.bad_extension
	cp ./cgi_tester  YoupiBanane/youpi.bla
	chmod +111 YoupiBanane/youpi.bla
	
	mkdir YoupiBanane/nop
	
	echo "youpi.bad_extension" > YoupiBanane/nop/youpi.bad_extension
	echo "other.pouic" > YoupiBanane/nop/other.pouic
	
	mkdir YoupiBanane/yeah
	echo "not_happy.bad_extension" > YoupiBanane/yeah/not_happy.bad_extension

clean:
	rm -rf YoupiBanane/
	rm tester -f
	rm cgi_tester -f
	rm *.json
