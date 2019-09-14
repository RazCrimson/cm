PATH=$PATH:$PWD/cm
replace=$PWD/clipboard
rm replace; touch replace
file=abc.txt
sed -s "rsclipboard/$replace/g" $file > $file.new
mv $file.new $file