#!/bin/bash
current=`date "+%Y-%m-%d %H:%M:%S"`  
timeStamp=`date -d "$current" +%s`   
#将current转换为时间戳，精确到毫秒  
startStamp=$((timeStamp*1000+`date "+%N"`/1000000)) 

mysql -uroot -h127.0.0.1 -P6666  -p123456 < sql > result 2>&1

current=`date "+%Y-%m-%d %H:%M:%S"`  
timeStamp=`date -d "$current" +%s`   
#将current转换为时间戳，精确到毫秒  
endStamp=$((timeStamp*1000+`date "+%N"`/1000000)) 
echo `expr $endStamp - $startStamp`
