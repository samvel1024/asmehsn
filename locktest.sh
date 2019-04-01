set -e
tests=1
while [ true ];
do
./meet_test1 10 20
./meet_test2 20
./meet_test3 10 100
echo -ne "Passed: $var%\033[0K\r"
var=$((var+1))
done