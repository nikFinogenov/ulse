#!/bin/bash

# cat [filename] | od -A n -t x1     // for show hex
home_dir=$HOME
uls_output="$home_dir/uls_output.txt"
ls_output="$home_dir/ls_output.txt"

mkdir temp_folder_for_test
mkdir temp_folder_for_test2
cd temp_folder_for_test

touch file1
touch file2
touch file3
ln -s file1 ln_file1
cd ..
ln -s temp ln_temp_file1

cd temp_folder_for_test2
touch file1
touch file2
touch file3
cd ..


./uls > $uls_output
ls > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls\" OK "
else
    echo "\"uls\" FALSE"
    #echo "$diff_res"
fi

./uls -R > $uls_output
ls -R > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -R\" OK "
else
    echo "\"uls -R\" FALSE"
    #echo "$diff_res"
fi

./uls -a > $uls_output
ls -a > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -a\" OK "
else
    echo "\"uls -a\" FALSE"
    #echo "$diff_res"
fi

./uls -l > $uls_output
ls -l > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -l\" OK "
else
    echo "\"uls -l\" FALSE"
    #echo "$diff_res"
fi

./uls -C > $uls_output
ls -C > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -C\" OK"
else
    echo "\"uls -C\" FALSE"
    #echo "$diff_res"
fi

./uls -1 > $uls_output
ls -1 > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -1\" OK "
else
    echo "\"uls -1\" FALSE"
    #echo "$diff_res"
fi

./uls -la > $uls_output
ls -la > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -la\" OK "
else
    echo "\"uls -la\" FALSE"
    #echo "$diff_res"
fi

./uls -al > $uls_output
ls -al > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -al\" OK "
else
    echo "\"uls -al\" FALSE"
    #echo "$diff_res"
fi

./uls -C1 > $uls_output
ls -C1 > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -C1\" OK "
else
    echo "\"uls -C1\" FALSE"
    #echo "$diff_res"
fi

./uls -1C > $uls_output
ls -1C > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -1C\" OK "
else
    echo "\"uls -1C\" FALSE"
    #echo "$diff_res"
fi

./uls -1C > $uls_output
ls -1C > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -1C\" OK "
else
    echo "\"uls -1C\" FALSE"
    #echo "$diff_res"
fi

./uls -1Cl > $uls_output
ls -1Cl > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -1Cl\" OK "
else
    echo "\"uls -1Cl\" FALSE"
    #echo "$diff_res"
fi

./uls -l1C > $uls_output
ls -l1C > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -l1C\" OK "
else
    echo "\"uls -l1C\" FALSE"
    #echo "$diff_res"
fi

./uls -Rl > $uls_output
ls -Rl > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -Rl\" OK "
else
    echo "\"uls -Rl\" FALSE"
    #echo "$diff_res"
fi

./uls -RaC > $uls_output
ls -RaC > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -RaC\" OK "
else
    echo "\"uls -RaC\" FALSE"
    #echo "$diff_res"
fi

./uls -l ln_temp_file1 > $uls_output
ls -l ln_temp_file1 > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -l ln_temp_file1\" OK "
else
    echo "\"uls -l ln_temp_file1\" FALSE"
    # echo "$diff_res"
fi

./uls -l ln_temp_file1/ > $uls_output
ls -l ln_temp_file1/ > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -l ln_temp_file1/\" OK "
else
    echo "\"uls -l ln_temp_file1/\" FALSE"
    #echo "$diff_res"
fi

./uls temp_folder_for_test temp_folder_for_test2 > $uls_output
ls temp_folder_for_test temp_folder_for_test2 > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -l temp_folder_for_test temp_folder_for_test2\" OK "
else
    echo "\"uls -l temp_folder_for_test temp_folder_for_test2\" FALSE"
    #echo "$diff_res"
fi

./uls temp_folder_for_test temp_folder_for_test2 > $uls_output
ls temp_folder_for_test temp_folder_for_test2 > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls temp_folder_for_test temp_folder_for_test2\" OK "
else
    echo "\"uls temp_folder_for_test temp_folder_for_test2\" FALSE"
    #echo "$diff_res"
fi
./uls -lh > $uls_output
ls -lh > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -lh\" OK "
else
    echo "\"uls -lh\" FALSE"
    #echo "$diff_res"
fi

./uls -Rlh > $uls_output
ls -Rlh > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -Rlh\" OK "
else
    echo "\"uls -Rlh\" FALSE"
    # echo "$diff_res"
fi

./uls -hl > $uls_output
ls -hl > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -hl\" OK "
else
    echo "\"uls -hl\" FALSE"
    # echo "$diff_res"
fi

./uls -l@ > $uls_output
ls -l@ > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -l@\" OK "
else
    echo "\"uls -l@\" FALSE"
    # echo "$diff_res"
fi

./uls -@l > $uls_output
ls -@l > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -@l\" OK "
else
    echo "\"uls -@l\" FALSE"
    # echo "$diff_res"
fi

./uls -l@h > $uls_output
ls -l@h > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -l@h\" OK "
else
    echo "\"uls -l@h\" FALSE"
    # echo "$diff_res"
fi

./uls -Rl@ > $uls_output
ls -Rl@ > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -Rl@\" OK "
else
    echo "\"uls -Rl@\" FALSE"
    # echo "$diff_res"
fi

./uls -m > $uls_output
ls -m > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -m\" OK "
else
    echo "\"uls -m\" FALSE"
    # echo "$diff_res"
fi

./uls -Rm > $uls_output
ls -Rm > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -Rm\" OK "
else
    echo "\"uls -Rm\" FALSE"
    # echo "$diff_res"
fi

./uls -G > $uls_output
ls -G > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -G\" OK "
else
    echo "\"uls -G\" FALSE"
    # echo "$diff_res"
fi

./uls -G1 > $uls_output
ls -G1 > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -G1\" OK "
else
    echo "\"uls -G1\" FALSE"
    # echo "$diff_res"
fi

./uls -1G > $uls_output
ls -1G > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -1G\" OK "
else
    echo "\"uls -1G\" FALSE"
    # echo "$diff_res"
fi

./uls -GC > $uls_output
ls -GC > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -GC\" OK "
else
    echo "\"uls -GC\" FALSE"
    # echo "$diff_res"
fi

./uls -Gl > $uls_output
ls -Gl > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -Gl\" OK "
else
    echo "\"uls -Gl\" FALSE"
    # echo "$diff_res"
fi

./uls -RGl > $uls_output
ls -RGl > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -RGl\" OK "
else
    echo "\"uls -RGl\" FALSE"
    # echo "$diff_res"
fi


./uls -RG1 > $uls_output
ls -RG1 > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -RG1\" OK "
else
    echo "\"uls -RG1\" FALSE"
    # echo "$diff_res"
fi

./uls -RGC > $uls_output
ls -RGC > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -RGC\" OK "
else
    echo "\"uls -RGC\" FALSE"
    # echo "$diff_res"
fi

./uls -RGm > $uls_output
ls -RGm > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -RGm\" OK "
else
    echo "\"uls -RGm\" FALSE"
    # echo "$diff_res"
fi

./uls -p > $uls_output
ls -p > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -p\" OK "
else
    echo "\"uls -p\" FALSE"
    # echo "$diff_res"
fi

./uls -lp > $uls_output
ls -lp > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -lp\" OK "
else
    echo "\"uls -lp\" FALSE"
    # echo "$diff_res"
fi

./uls -Cp > $uls_output
ls -Cp > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -Cp\" OK "
else
    echo "\"uls -Cp\" FALSE"
    # echo "$diff_res"
fi

./uls -pC > $uls_output
ls -pC > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -pC\" OK "
else
    echo "\"uls -pC\" FALSE"
    # echo "$diff_res"
fi

./uls -p1 > $uls_output
ls -p1 > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -p1\" OK "
else
    echo "\"uls -p1\" FALSE"
    # echo "$diff_res"
fi

./uls -pm > $uls_output
ls -pm > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -pm\" OK "
else
    echo "\"uls -pm\" FALSE"
    # echo "$diff_res"
fi

./uls -pGm > $uls_output
ls -pGm > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -p\" OK "
else
    echo "\"uls -p\" FALSE"
    # echo "$diff_res"
fi

./uls -Rp > $uls_output
ls -Rp > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -Rp\" OK "
else
    echo "\"uls -Rp\" FALSE"
    # echo "$diff_res"
fi

# Очистить временные файлы
rm $uls_output
rm $ls_output
rm -r temp_folder_for_test
rm -r temp_folder_for_test2
rm ln_temp_file1
