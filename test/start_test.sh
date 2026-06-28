#!/bin/bash
rm -rf ../webserv
make -C ../
echo "interpreter = $0"

echo "########################## TEST 0 ##################################"
bash ./00_basic_config_file.sh

echo "########################## TEST 1 ##################################"
bash ./01_test_basic_host.sh


echo "########################## TEST 2 ##################################"
bash ./02_location_basic_test.sh

echo "########################## TEST 3 ##################################"
bash ./03_no_methods_allowed.sh

echo "########################## TEST 4 ##################################"
bash ./04_directory_listing_on.sh

echo "########################## TEST 5 ##################################"
bash ./05_directory_listing_off_no_index.sh

echo "########################## TEST 6 ##################################"
bash ./06_basic_test_index.sh

echo "########################## TEST 7 ##################################"
bash ./07_basic_cgi.sh

echo "########################## TEST 7 bis ##################################"
bash ./07_bis_second_cgi_test.sh

echo "########################## TEST 8 ##################################"
bash ./08_basic_error_page.sh

echo "########################## TEST 9 ##################################"
bash ./09_forbidden_port.sh

echo "########################## TEST 10 ##################################"
bash ./10_basic_redirection.sh

echo "########################## TEST 11 ##################################"
bash ./11_too_large_body.sh

echo "########################## TEST 12 ##################################"
bash ./12_test_delete_request.sh
