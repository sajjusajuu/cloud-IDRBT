# IDRBT - Cloud Retrieval
# Webpage: https://www.idrbt.ac.in
# Contact: hussainsajid@acm.org
# Institute of Development and Research in Banking Technology
# 
# The Original Code is q_vector.py
# This program generates the query vector from the given keywords. The keywords are stored as a list 
# in a json file named given as argument
#
# 
# Running : $ 
# Output :
#
# Contributor(s):
#  Sajid Hussain

import lib

if len(sys.argv) != 2:
	print 'Invalid argument'

keywords = set(lib.load(sys.argv[1]))
dict_list = lib.load('dictionary.json')

q_vec = []

for word in dict_list:
	if word in keywords:
		q_vec.append(1)
	else
		q_vec.append(0)
# dump the query vector to a json file.
