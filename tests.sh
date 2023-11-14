#!/bin/bash

# cat [filename] | od -A n -t x1     // for show hex
home_dir=$HOME
uls_output="$home_dir/uls_output.txt"
ls_output="$home_dir/ls_output.txt"

./uls > $uls_output
ls > $ls_output

diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls\" OK "
else
    echo "\"uls\" FALSE"
    echo "$diff_res"
fi

./uls -R > $uls_output
ls -R > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -R\" OK "
else
    echo "\"uls -R\" FALSE"
    echo "$diff_res"
fi

./uls -a > $uls_output
ls -a > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -a\" OK "
else
    echo "\"uls -a\" FALSE"
    echo "$diff_res"
fi

./uls -l > $uls_output
ls -l > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -l\" OK "
else
    echo "\"uls -l\" FALSE"
    echo "$diff_res"
fi

./uls -C > $uls_output
ls -C > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -C\" OK"
else
    echo "\"uls -C\" FALSE"
    echo "$diff_res"
fi

./uls -1 > $uls_output
ls -1 > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -1\" OK "
else
    echo "\"uls -1\" FALSE"
    echo "$diff_res"
fi

./uls -la > $uls_output
ls -la > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -la\" OK "
else
    echo "\"uls -la\" FALSE"
    echo "$diff_res"
fi

./uls -al > $uls_output
ls -al > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -al\" OK "
else
    echo "\"uls -al\" FALSE"
    echo "$diff_res"
fi

./uls -C1 > $uls_output
ls -C1 > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -C1\" OK "
else
    echo "\"uls -C1\" FALSE"
    echo "$diff_res"
fi

./uls -1C > $uls_output
ls -1C > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -1C\" OK "
else
    echo "\"uls -1C\" FALSE"
    echo "$diff_res"
fi

./uls -1C > $uls_output
ls -1C > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -1C\" OK "
else
    echo "\"uls -1C\" FALSE"
    echo "$diff_res"
fi

./uls -1Cl > $uls_output
ls -1Cl > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -1Cl\" OK "
else
    echo "\"uls -1Cl\" FALSE"
    echo "$diff_res"
fi

./uls -l1C > $uls_output
ls -l1C > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -l1C\" OK "
else
    echo "\"uls -l1C\" FALSE"
    echo "$diff_res"
fi

./uls -Rl > $uls_output
ls -Rl > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -Rl\" OK "
else
    echo "\"uls -Rl\" FALSE"
    echo "$diff_res"
fi

# ./uls -RaC > $uls_output
# ls -RaC > $ls_output
# diff_res=$(diff $uls_output $ls_output)
# if [ $? -eq 0 ]; then
#     echo "\"uls -RaC\" OK "
# else
#     echo "\"uls -RaC\" FALSE"
#     echo "$diff_res"
# fi

# Очистить временные файлы
rm $uls_output
rm $ls_output
