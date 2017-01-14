#!/bin/bash


hash docker &>/dev/null && 
    echo "All programs installed" ||
    exit 

#echo Building test image
#docker rm test1 &> /dev/null
#docker build -t test1 ./ &> /dev/null

echo "--------------------------------------------"
echo "|      Test 3, two file transfer        |"
echo "--------------------------------------------"
docker run  --name a1   -v $(pwd)/..:/workVol test1  /workVol/TinTorrent /tin /config.json &> ./a1_log &
docker run  --name a2  -v $(pwd)/..:/workVol test1  /workVol/TinTorrent /tin /config.json &> ./a2_log &

sleep 1 

docker cp ./a1_file a1:/tin/a1_file 
docker cp ./c1_file a1:/tin/c1_file 

sleep  120
#

docker cp a2:/tin/a1_file.10001 ./a2_file
docker cp a2:/tin/c1_file.10003 ./c2_file
if [ -f a2_file ];
	then
	   echo "File $FILE exists. Test OK"
	else
	   echo "File $FILE does not exist, Test FAILED"
fi

if [ -f c2_file ];
	then
	   echo "File $FILE exists. Test OK"
	else
	   echo "File $FILE does not exist, Test FAILED"
fi


docker kill a1 a2 &> /dev/null

docker rm a1 a2 &> /dev/null

#rm ./a2_file &> /dev/null

