#
# IDRBT - Cloud Retrieval
# Webpage: https://www.idrbt.ac.in
# Contact: hussainsajid@acm.org
# Institute of Development and Research in Banking Technology
# 
# The Original Code is listing_files.sh
# This file lists all the files in the given directory recursively.
#
# Note: The files in the directory should have a single name without any spaces in between
# 
# Running : $ ./listing_files.sh <absolute_path>
# Ex : ./listing_files.sh /home/sajid/maildir   
# Output : Lists all the files on to the output
#
# Contributor(s):
#  Sajid Hussain


listfile() {
	if [ -f $1 ]; then 
		echo $1
	else
		for var in $(ls $1); do
			dir=$1/$var
			if [ -d $dir ]
			then
				#echo $dir is a directory
				listfile $dir
			else
				echo $dir
			fi
		done
	fi
}

if [ $# != 1 ]; then
	echo "Please enter the directory to be indexed"
fi	

if [ -d $1 ] || [ -f $1 ]; then
	listfile $1
else
	echo "$1 is a invalid directory or file"
fi