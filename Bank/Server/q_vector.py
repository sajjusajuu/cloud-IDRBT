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

import os, zipfile
import json, nltk

from ctypes import *

def gen_query(qlist):
	pattern = r'''(?x)
		([A-Z]\.)+
		| \w+(-\w+)*
		| \$?\d+(\.\d+)?%? 
		| \.\.\.
		| [][.,;"'?():-_`] 
	'''

	# tokens is a list containing all the words in the contents of the mail in list

	qlist = nltk.regexp_tokenize(qlist, pattern)
	qlist = [i.lower() for i in qlist]
	dict_file = open('dictionary.json', 'r')
	dict_list = json.load(dict_file)
	num_words_dict = len(dict_list)
	keyw_set = set(qlist)
	q_vec = []

	for word in dict_list:
		if word in keyw_set:
			q_vec.append(1)
		else:
			q_vec.append(0)


	crypt_lib = CDLL("/home/sajid/cloud-IDRBT/Bank/lib/crypting_vector.so")
	q_arr = (c_int * len(q_vec))(*q_vec)
	rand_seed = 4321
	crypt_lib.crypt_vector(q_arr, c_int(num_words_dict), c_int(1),  c_int(rand_seed))
	
	crpzip = zipfile.ZipFile('query.zip', 'w')
	crpzip.write(os.path.join("crypt_1.key"))
	crpzip.write(os.path.join("crypt_2.key"))
	crpzip.close()
	os.remove("crypt_1.key")
	os.remove("crypt_2.key")

