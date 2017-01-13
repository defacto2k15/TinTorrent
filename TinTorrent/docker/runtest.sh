#!/bin/bash


hash docker &>/dev/null && 
    echo "All programs installed" ||
    exit 

echo Building test image
docker rm test1 &> /dev/null
docker build -t test1 ./ &> /dev/null

echo "--------------------------------------------"
echo "|      Test 1, simple file transfer        |"
echo "--------------------------------------------"
docker run  --name a1 test1 /TinTorrent /tin /config.json &> ./a1_log &
docker run  --name a2 test1 /TinTorrent /tin /config.json &> ./a2_log &

sleep 1 

docker cp ./a1_file a1:/tin/a1_file 

sleep 120


docker cp a2:/tin/a1_file ./a2_file
if [ -f a2_file ];
	then
	   echo "File $FILE exists. Test OK"
	else
	   echo "File $FILE does not exist, Test FAILED"
fi


docker kill a1 a2 &> /dev/null

docker rm a1 a2 &> /dev/null

rm ./a2_file &> /dev/null

