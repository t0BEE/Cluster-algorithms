#! /usr/bin/env bash
set -e
program=$1

runs=$2
task_name=$3
set OMP_DYNAMIC=0

data=(blobs circles noise)
page_size=4096

echo "REQUEST CPU"
touch ~/REQUEST_CPU
sleep 15

echo "CPU REQUESTED"

for i in ${!data[*]}
do
        echo "Doing single ${data[$i]}" >&2
        $program "${task_name}\$${data[$i]}\$0" ";" "${runs}" "data/${data[$i]}.data" "2" ";" "${page_size}" 2>&1
done

rm ~/REQUEST_CPU
