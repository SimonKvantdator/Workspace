echo "10 copies time" >> time.txt
{ time {
for x in {1..10}
do
cp  -rp ~/include/ ~/include_copy
done
}
} 2>> time.txt
