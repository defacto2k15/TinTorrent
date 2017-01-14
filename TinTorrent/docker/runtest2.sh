#!/bin/bash


hash docker &>/dev/null && 
    echo "All programs installed" ||
    exit 

#echo Building test image
#docker rm test1 &> /dev/null
#docker build -t test1 ./ &> /dev/null

echo "--------------------------------------------"
echo "|      Test 2, simple file banning by testScenario        |"
echo "--------------------------------------------"

cp a2Scenario/* a2TestDirectory/

docker run  --name a1   -v $(pwd)/..:/workVol                                                                                     test1  /workVol/TinTorrent /tin /config.json &> ./a1_log &
docker run  --name a2   -v $(pwd)/..:/workVol -v $(pwd)/a2TestDirectory/:/testData test1  /workVol/TinTorrent /tin /testData/test2a2config.json  /testData/test2a2Scenario.json &> ./a2_log &

sleep 1 

docker cp ./a1_file a1:/tin/a1_file 
docker cp ./a1_file a2:/tin/a1_file

sleep 70

#upewnij się, że nie ma w obu containerach takiego pliku, a w a2TestDirectory jest plik getRevertedResources.txt z opisem zasobu zakazanego

docker kill a1 a2 &> /dev/null

docker rm a1 a2 &> /dev/null

#rm ./a2_file &> /dev/null

