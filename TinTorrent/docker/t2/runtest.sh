#!/bin/bash
MASZYNY=10

hash docker &>/dev/null && 
    echo "All programs installed" ||
    exit 

echo Building test image
docker rm test1 &> /dev/null
docker build -t test1 ./ &> /dev/null

echo "revertedResource test"
docker run  --name a1 test1 /TinTorrent /tin /config.json /scenario.json &> ./logs/a1_log &

for maszyna in `seq 2 $MASZYNY`;
do
	echo "Starting $maszyna docker image"
	docker run  --name a$maszyna test1 /TinTorrent /tin /config.json  &> ./logs/a$maszyna\_log &
done
		

sleep 5

docker cp ./a1_file a1:/tin/a1_file

sleep 100

docker cp a1:/getRevertedResources.txt ./getRevertedResources.txt

echo "--------------------------------------------------------------------------------"
cat ./getRevertedResources.txt
echo -e "\n"
for maszyna in `seq 1 $MASZYNY`;
do
	mkdir a$maszyna &> /dev/null
	docker cp a$maszyna:/tin/a1_file.10001 ./a$maszyna &> /dev/null
	docker cp a$maszyna:/tin/a1_file.10001.metadata ./a$maszyna &> /dev/null
done
ls ./a*
echo "--------------------------------------------------------------------------------"


for maszyna in `seq 1 $MASZYNY`;
do
	docker kill a$maszyna &> /dev/null
	docker rm a$maszyna &> /dev/null
done

