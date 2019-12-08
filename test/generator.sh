#!/bin/bash
echo "use test;"
for((i=1;i<1000;i++));
do
	echo "insert into t1 values($i);";
done
for((i=1;i<1000;i++));
do
	echo "select id from t1 where id=$i;";
done
for((i=1;i<1000;i++));
do
	echo "delete from t1 where id=$i;";
done
