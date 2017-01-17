#!/bin/bash
MASZYNY=10
PLIKI=5

hash docker &>/dev/null && 
    echo "All programs installed" ||
    exit 

echo Building test image
docker rm test1 &> /dev/null
docker build -t test1 ./ &> /dev/null

echo "getResourcesInOtherClients test"
docker run  --name a1 test1 /TinTorrent /tin /config.json /scenario.json &> ./logs/a1_log &

for maszyna in `seq 2 $MASZYNY`;
do
	echo "Starting $maszyna docker image"
	docker run  --name a$maszyna test1 /TinTorrent /tin /config.json  &> ./logs/a$maszyna\_log &
done
		

sleep 5

for maszyna in `seq 2 $MASZYNY`;
do
	echo "Doing $maszyna docker instance"
	mkdir a$maszyna
	for plik in `seq 1 $PLIKI`;
	do
		echo "Doint $plik file"
		echo "$maszyna $plik" > ./a$maszyna/a$maszyna\_$plik
		docker cp ./a$maszyna/a$maszyna\_$plik a$maszyna:/tin/a$maszyna\_$plik
	done
done

sleep 150


docker cp a1:/getResourcesInOtherClients.txt ./getResourcesInOtherClients.txt

echo "--------------------------------------------------------------------------------"
cat getResourcesInOtherClients.txt
echo -e "\n"
echo "--------------------------------------------------------------------------------"


for maszyna in `seq 1 $MASZYNY`;
do
	docker kill a$maszyna &> /dev/null
	docker rm a$maszyna &> /dev/null
done

