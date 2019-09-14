#bash script to add to path, clear clipboard and compile the file as cm
PATH=$PATH:$PWD/cm
path=$PWD/clipboard
rm path; touch path
file=cm1.c
sed -i 's|clipboard_path_added_by_bash_script|'$path'|g' $file > $file.new
#rm -f $file
#mv $file.new $file
gcc -O $file -o cm