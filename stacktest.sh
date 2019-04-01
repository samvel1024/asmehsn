var=0
while [ true ];
do
    var=$((var+1))

    ./testgen ${var} 100 > test.in
    timeout 2 ./cimpl 1 $(cat test.in)  > cimpl.outs
    if [ $? -eq 0 ]; then
        timeout 2 ./asmimpl 1 $(cat test.in)  > asmimpl.out
        if [ $? -ne 0 ]; then
            echo "ERROR!!!!"
            exit 1
        fi
        echo -ne "Passed: $var \033[0K\r"
    fi
done