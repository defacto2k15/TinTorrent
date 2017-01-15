#!/bin/bash


hash docker &>/dev/null && 
    echo "All programs installed" ||
    exit 

echo Building test image
docker rm test1 &> /dev/null
docker build -t test1 ./ &> /dev/null

echo "--------------------------------------------"
echo "|   Interactive test, play with ncurses    |"
echo "--------------------------------------------"

docker run --name a1 -v $(pwd)/..:/workVol test1 /bin/bash -c "sleep 9999" &
docker run --name a2 -v $(pwd)/..:/workVol test1 /bin/bash -c "sleep 9999" &

sleep 1

docker cp ./a1_file a1:/tin/a1_file.txt
docker cp ./c1_file a2:/tin/c1_file.txt
docker cp ./config_interactive.json a1:/config.json
docker cp ./config_interactive.json a2:/config.json
echo "Copied files into dockers"
echo "-----"

sudo x-terminal-emulator -e "sudo docker exec -it a1 /bin/bash -c 'export TERM=xterm && ./TinTorrent /tin /config.json 2>/a1_log'" &
sudo x-terminal-emulator -e "sudo docker exec -it a2 /bin/bash -c 'export TERM=xterm && ./TinTorrent /tin /config.json 2>/a2_log'" &

sleep 1 
echo "-----"

echo "Press 'q' to stop test"
x='';
while [[ "$x" != "q" ]]; do read -n1 x; done
echo

docker cp a1:/a1_log ./a1_log 
docker cp a2:/a2_log ./a2_log
echo "Logs copied from dockers"

docker kill a1 a2 &> /dev/null

docker rm a1 a2 &> /dev/null