#!/bin/sh
NAMES='clear composite eraseall erase find insert iterate'
for NAME in ${NAMES};
do
	echo "${NAME}..."
	"./benchmark-${NAME}" > "${NAME}.out"
	'./bench_to_csv.py' < "${NAME}.out" > "${NAME}.csv"
done
