#!/bin/bash


hash docker &>/dev/null && 
    echo "All programs installed" ||
    exit 

echo Building test image
docker rm test1 &> /dev/null
docker build -t test1 ./ &> /dev/null

echo "--------------------------------------------"
echo "|   Interactive test, with docker web, play with ncurses    |"
echo "--------------------------------------------"

docker rm -f a1 a2 &> /dev/null

docker run  --name a1 test1 /TinTorrent /tin /config.json &> ./a1_log &
docker run  --name a2 test1 /TinTorrent /tin /config.json &> ./a2_log &

echo "Dockerowe torrenty wystartowane, teraz kopiujemy do nich pliki "

docker cp ./a1_file a1:/tin/a1_file.txt
docker cp ./c1_file a2:/tin/c1_file.txt

rm dockerWebTestWorkingDir/*
cp fromHostFile dockerWebTestWorkingDir/fromHostFile

echo "Dockerowe torrenty wystartowane, teraz odpalenie lokalnego programu"

../TinTorrent ./dockerWebTestWorkingDir ./dockerWebTestConfig.json  2> localHostLog


echo "Press 'q' to stop test"
x='';
while [[ "$x" != "q" ]]; do read -n1 x; done
echo

docker rm -f a1 a2 &> /dev/null