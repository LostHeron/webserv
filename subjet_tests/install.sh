rm tester
wget https://cdn.intra.42.fr/document/document/46302/tester
chmod +111 tester

rm cgi_tester
wget https://cdn.intra.42.fr/document/document/46303/cgi_tester
chmod +111 cgi_tester

rm -rf YoupiBanane
mkdir YoupiBanane
echo "youpi.bad_extension" > YoupiBanane/youpi.bad_extension
cp ./cgi_tester  YoupiBanane/youpi.bla
chmod +111 YoupiBanane/youpi.bla

mkdir YoupiBanane/nop

echo "youpi.bad_extension" > YoupiBanane/nop/youpi.bad_extension
echo "other.pouic" > YoupiBanane/nop/other.pouic

mkdir YoupiBanane/Yeah
echo "not_happy.bad_extension" > YoupiBanane/Yeah/not_happy.bad_extension




OLD_IFS=$IFS
IFS=""
echo "\"host\":
[
	{
	    \"listen\":4343,
		\"index\":\"youpi.bad_extension\",
	    \"name\": \"test\",
		\"request\":[\"GET\"],
		\"extension\":[\".bla\"],
	    \"root\":\"$PWD/YoupiBanane\",
		\"max body\": 1000000000000,
		\"location\":
		[
			{
				\"name\":\"/post_body\",
				\"extension\":[\".bla\"],
				\"request\":[\"POST\"]
			},
			{
				\"name\":\"/directory\",
				\"alias\":\"$PWD/YoupiBanane\",
				\"extension\":[\".bla\"],
				\"index\":\"youpi.bad_extension\",
				\"request\":[\"GET\", \"POST\"]
			}
		]
	}
]" > config_file.json
