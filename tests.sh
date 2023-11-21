#!/bin/bash

# cat [filename] | od -A n -t x1     // for show hex
home_dir=$HOME
uls_output="$home_dir/uls_output.txt"
ls_output="$home_dir/ls_output.txt"

mkdir temp_folder_for_test
mkdir temp_folder_for_test2
mkdir temp_folder_empty
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
    echo "\"uls -pGm\" OK "
else
    echo "\"uls -pGm\" FALSE"
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

./uls -Rpm > $uls_output
ls -Rpm > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -Rpm\" OK "
else
    echo "\"uls -Rpm\" FALSE"
    # echo "$diff_res"
fi

./uls -F > $uls_output
ls -F > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -F\" OK "
else
    echo "\"uls -F\" FALSE"
    # echo "$diff_res"
fi

./uls -Fl > $uls_output
ls -Fl > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -Fl\" OK "
else
    echo "\"uls -Fl\" FALSE"
    # echo "$diff_res"
fi

./uls -RF > $uls_output
ls -RF > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -RF\" OK "
else
    echo "\"uls -RF\" FALSE"
    # echo "$diff_res"
fi

./uls -RFm > $uls_output
ls -RFm > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -RFm\" OK "
else
    echo "\"uls -RFm\" FALSE"
    # echo "$diff_res"
fi

./uls -S > $uls_output
ls -S > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -S\" OK "
else
    echo "\"uls -S\" FALSE"
    # echo "$diff_res"
fi

./uls -Sr > $uls_output
ls -Sr > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -Sr\" OK "
else
    echo "\"uls -Sr\" FALSE"
    # echo "$diff_res"
fi

./uls -Sl > $uls_output
ls -Sl > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -Sl\" OK "
else
    echo "\"uls -Sl\" FALSE"
    # echo "$diff_res"
fi

./uls -SC > $uls_output
ls -SC > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -SC\" OK "
else
    echo "\"uls -SC\" FALSE"
    # echo "$diff_res"
fi

./uls -S1 > $uls_output
ls -S1 > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -S1\" OK "
else
    echo "\"uls -S1\" FALSE"
    # echo "$diff_res"
fi

./uls -1S > $uls_output
ls -1S > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -1S\" OK "
else
    echo "\"uls -1S\" FALSE"
    # echo "$diff_res"
fi

./uls -Sm > $uls_output
ls -Sm > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -Sm\" OK "
else
    echo "\"uls -Sm\" FALSE"
    # echo "$diff_res"
fi

./uls -SR > $uls_output
ls -SR > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -SR\" OK "
else
    echo "\"uls -SR\" FALSE"
    # echo "$diff_res"
fi

./uls -RS > $uls_output
ls -RS > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -RS\" OK "
else
    echo "\"uls -RS\" FALSE"
    # echo "$diff_res"
fi

./uls -SRCGm > $uls_output
ls -SRCGm > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -SRCGm\" OK "
else
    echo "\"uls -SRCGm\" FALSE"
    # echo "$diff_res"
fi

./uls -t > $uls_output
ls -t > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -t\" OK "
else
    echo "\"uls -t\" FALSE"
    # echo "$diff_res"
fi

./uls -c > $uls_output
ls -c > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -c\" OK "
else
    echo "\"uls -c\" FALSE"
    # echo "$diff_res"
fi

./uls -u > $uls_output
ls -u > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -u\" OK "
else
    echo "\"uls -u\" FALSE"
    # echo "$diff_res"
fi

./uls -U > $uls_output
ls -U > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -U\" OK "
else
    echo "\"uls -U\" FALSE"
    # echo "$diff_res"
fi

./uls -cu > $uls_output
ls -cu > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -cu\" OK "
else
    echo "\"uls -cu\" FALSE"
    # echo "$diff_res"
fi

./uls -UcS > $uls_output
ls -UcS > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -UcS\" OK "
else
    echo "\"uls -UcS\" FALSE"
    # echo "$diff_res"
fi

./uls -RUc > $uls_output
ls -RUc > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -RUc\" OK "
else
    echo "\"uls -RUc\" FALSE"
    # echo "$diff_res"
fi

./uls -f > $uls_output
ls -f > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -f\" OK "
else
    echo "\"uls -f\" FALSE"
    # echo "$diff_res"
fi


./uls -fR > $uls_output
ls -fR > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -fR\" OK "
else
    echo "\"uls -fR\" FALSE"
    # echo "$diff_res"
fi

./uls -ft > $uls_output
ls -ft > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -ft\" OK "
else
    echo "\"uls -ft\" FALSE"
    # echo "$diff_res"
fi

./uls -tfa > $uls_output
ls -tfa > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -tfa\" OK "
else
    echo "\"uls -tfa\" FALSE"
    # echo "$diff_res"
fi

./uls -tR > $uls_output
ls -tR > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -tR\" OK "
else
    echo "\"uls -tR\" FALSE"
    # echo "$diff_res"
fi

./uls -tR > $uls_output
ls -tR > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -Rt\" OK "
else
    echo "\"uls -Rt\" FALSE"
    # echo "$diff_res"
fi

./uls -lg > $uls_output
ls -lg > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -lg\" OK "
else
    echo "\"uls -lg\" FALSE"
    # echo "$diff_res"
fi

./uls -lo > $uls_output
ls -lo > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -lo\" OK "
else
    echo "\"uls -lo\" FALSE"
    # echo "$diff_res"
fi

./uls -log > $uls_output
ls -log > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -log\" OK "
else
    echo "\"uls -log\" FALSE"
    # echo "$diff_res"
fi

./uls -og > $uls_output
ls -og > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -og\" OK "
else
    echo "\"uls -og\" FALSE"
    # echo "$diff_res"
fi

./uls -le > $uls_output
ls -le > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -le\" OK "
else
    echo "\"uls -le\" FALSE"
    # echo "$diff_res"
fi

./uls -elg > $uls_output
ls -elg > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -elg\" OK "
else
    echo "\"uls -elg\" FALSE"
    # echo "$diff_res"
fi

./uls -go > $uls_output
ls -go > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -go\" OK "
else
    echo "\"uls -go\" FALSE"
    # echo "$diff_res"
fi

./uls -x > $uls_output
ls -x > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -x\" OK "
else
    echo "\"uls -x\" FALSE"
    echo "$diff_res"
fi

./uls -lx > $uls_output
ls -lx > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -lx\" OK "
else
    echo "\"uls -lx\" FALSE"
    # echo "$diff_res"
fi

./uls -lxC > $uls_output
ls -lxC > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -lxC\" OK "
else
    echo "\"uls -lxC\" FALSE"
    # echo "$diff_res"
fi

./uls -lxm > $uls_output
ls -lxm > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -lxm\" OK "
else
    echo "\"uls -lxm\" FALSE"
    # echo "$diff_res"
fi

./uls -mmx > $uls_output
ls -mmx > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -mmx\" OK "
else
    echo "\"uls -mmx\" FALSE"
    # echo "$diff_res"
fi

./uls -xml > $uls_output
ls -xml > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -xml\" OK "
else
    echo "\"uls -xml\" FALSE"
    # echo "$diff_res"
fi

./uls -Rx > $uls_output
ls -Rx > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -Rx\" OK "
else
    echo "\"uls -Rx\" FALSE"
    # echo "$diff_res"
fi

./uls -RxmC > $uls_output
ls -RxmC > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -RxmC\" OK "
else
    echo "\"uls -RxmC\" FALSE"
    # echo "$diff_res"
fi

./uls -n > $uls_output
ls -n > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -n\" OK "
else
    echo "\"uls -n\" FALSE"
    # echo "$diff_res"
fi

./uls -nC > $uls_output
ls -nC > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -nC\" OK "
else
    echo "\"uls -nC\" FALSE"
    # echo "$diff_res"
fi

./uls -ln > $uls_output
ls -ln > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -ln\" OK "
else
    echo "\"uls -ln\" FALSE"
    # echo "$diff_res"
fi

./uls -xn > $uls_output
ls -xn > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -xn\" OK "
else
    echo "\"uls -xn\" FALSE"
    # echo "$diff_res"
fi

./uls -i > $uls_output
ls -i > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -i\" OK "
else
    echo "\"uls -i\" FALSE"
    # echo "$diff_res"
fi

./uls -li > $uls_output
ls -li > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -li\" OK "
else
    echo "\"uls -li\" FALSE"
    # echo "$diff_res"
fi

./uls -il > $uls_output
ls -il > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -il\" OK "
else
    echo "\"uls -il\" FALSE"
    # echo "$diff_res"
fi

./uls -iC > $uls_output
ls -iC > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -iC\" OK "
else
    echo "\"uls -iC\" FALSE"
    # echo "$diff_res"
fi

./uls -ix > $uls_output
ls -ix > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -ix\" OK "
else
    echo "\"uls -ix\" FALSE"
    # echo "$diff_res"
fi

./uls -i1 > $uls_output
ls -i1 > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -i1\" OK "
else
    echo "\"uls -i1\" FALSE"
    # echo "$diff_res"
fi

./uls -i1 > $uls_output
ls -i1 > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -i1\" OK "
else
    echo "\"uls -i1\" FALSE"
    # echo "$diff_res"
fi

./uls -iln > $uls_output
ls -iln > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -iln\" OK "
else
    echo "\"uls -iln\" FALSE"
    # echo "$diff_res"
fi

./uls -im > $uls_output
ls -im > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -im\" OK "
else
    echo "\"uls -im\" FALSE"
    # echo "$diff_res"
fi

./uls test_folder_empty > $uls_output
ls test_folder_empty > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls empty folder\" OK "
else
    echo "\"uls empty folder\" FALSE"
    # echo "$diff_res"
fi

./uls -l test_folder_empty > $uls_output
ls -l test_folder_empty > $ls_output
diff_res=$(diff $uls_output $ls_output)
if [ $? -eq 0 ]; then
    echo "\"uls -l empty folder\" OK "
else
    echo "\"uls -l empty folder\" FALSE"
    # echo "$diff_res"
fi

# Очистить временные файлы
rm $uls_output
rm $ls_output
rm -r temp_folder_for_test
rm -r temp_folder_for_test2
rm -r test_folder_empty
rm ln_temp_file1
