# IDRBT - Cloud Retrieval
# Webpage: https://www.idrbt.ac.in
# Contact: hussainsajid@acm.org
# Institute of Development and Research in Banking Technology
# 
# The Original Code is binary_vector.py
# This program generates all the dictionary words from the files in list.txt
#
# Note: Required Pakages: NLTK from http://www.nltk.org/
# 
# Running : $ phthon binary_vector.py list.txt
# Output : A binary vector datasructure for each file
#
# Contributor(s):
#  Sajid Hussain

import sys, re, json
import nltk, pprint


if len(sys.argv) != 2:
	print """\
	Error in binary_vector.py
	Usage: python binary_vector.py list.txt
	"""
file_l = open(sys.argv[1], 'r')
dict_list = open('dictionary.json', 'r') 

dictionary = json.load(dict_list)

def make_binary_vector(f, adrs):
	set_file = set()
	binary_vector = []
	s = f.read()
	match = re.search(r'\r\n\r\n', s)
	head = s[: match.start()]
	pattern = r'''(?x)
		([A-Z]\.)+
		| \w+(-\w+)*
		| \$?\d+(\.\d+)?%? 
		| \.\.\.
		| [][.,;"'?():-_`] 
	'''

	tokens = nltk.regexp_tokenize(s, pattern)
	tokens = [i.lower() for i in tokens]
	tokens = [i for i in tokens if isword(i)]

	wnl = nltk.WordNetLemmatizer()
	tokens = [wnl.lemmatize(t) for t in tokens]

	for word in tokens:
		if word not in stopwords:
			if wordnet.synsets(word):
				#print word
				set_file.add(word)

	for word in dictionary:
		if word in set_file:
			binary_vector.append(1)
		else:
			binary_vector.append(0)
	f = open('bin_vec/'+formt(adrs), 'w')
	json.dump(binary_vector, f)


for line in file_l:							
	f = open(line.strip(), 'r')
	make_binary_vector(f, line.strip())
	print 'dictionary has', len(dictionary), 'elements'