#!/bin/bash
#check if there is no input from user.
if [ $# -gt 0 ]; then
# Here we var with the title of the files - first pipe replace \n instead } - second pipe delete unnecessary signs - Third pipe create delimiters -awk extract the data we need.(only odd coulums)
title=$(curl "https://opensky-network.org/api/flights/arrival?airport=EDDF&begin=$(date -d 'yesterday 00:00' '+%s')&end=$(date '+%s')" | sed 's/}/\n/g' | sed 's/[{\[]\|"\|}//g' | sed 's/,/:/g' | awk -F':' 'NR==1 {for (i=1;i<12;i+=2) printf $i ","}')

#Create the flight dict
mkdir flightDB 
for arg in "$@"; do

#Here we save the response in variable for the if condition 
 response=$(curl -s -o /dev/null -w "%{http_code}" $"https://opensky-network.org/api/flights/arrival?airport=$arg&begin=$(date -d 'yesterday 00:00' '+%s')&end=$(date '+%s')")
  if [ $response -eq 200 ]; then  #if response eq 200 then the icao exist, then we create the dirctory.
  mkdir flightDB/$arg   
  
  echo -n "$title" > flightDB/$arg/"$arg.arv"  # We insert the title to the file then we modify the data from the curl command like we did to the title the we extract the data we need with awk command.
  curl "https://opensky-network.org/api/flights/arrival?airport=$arg&begin=$(date -d 'yesterday 00:00' '+%s')&end=$(date '+%s')" | sed 's/}/\n/g' | sed 's/[{"]//g' | sed 's/[\[,]/:/g' | 
  awk -F':' '{printf "\n";for(i=3;i<=14;i+=2) printf $i ","}' >> flightDB/$arg/"$arg.arv"
  sed -i -e 's/.$//' -e 's/[ \t]//g' flightDB/$arg/"$arg.arv" #here he delete the space and the extra , 

  echo -n "$title" > flightDB/$arg/"$arg.dpt" # We insert the title to the file then we modify the data from the curl command like we did to the title the we extract the data we need with awk command.
  curl "https://opensky-network.org/api/flights/departure?airport=$arg&begin=$(date -d 'yesterday 00:00' '+%s')&end=$(date '+%s')" | sed 's/}/\n/g' | sed 's/[{"]//g' | sed 's/[\[,]/:/g' | 
  awk -F':' '{printf "\n";for(i=3;i<=14;i+=2) printf $i ","}' >> flightDB/$arg/"$arg.dpt"
  sed -i -e 's/.$//' -e 's/[ \t]//g' flightDB/$arg/"$arg.dpt" #here he delete the space and the extra  , 
  sed -i '$ d' flightDB/$arg/"$arg.dpt" flightDB/$arg/"$arg.arv"
 else
    echo "$arg not exist in our DB. Failed to retrieve data"
 fi
   done 
 else 
 echo "No arguments provided.try again"  
fi


