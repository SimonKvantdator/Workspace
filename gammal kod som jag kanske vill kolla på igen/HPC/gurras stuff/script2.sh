echo "copytime" >> time.txt
{ time {
for x in {1..10}
do
cp  -r /run/mount/scratch/hpcuser281/include/ /run/mount/scratch/hpcuser281/include_copy
done
}
} 2>> time.txt
