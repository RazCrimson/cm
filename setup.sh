#bash script to add to path, clear clipboard and compile the file as cm
PATH=$PATH:$PWD/cm
path=$PWD/.clipboard
rm $path; touch $path
file=cm.c
sed -i 's|clipboard_path_added_by_setup.sh|'$path'|g' $file > output.c 
gcc output.c -o cm
rm -f output.c