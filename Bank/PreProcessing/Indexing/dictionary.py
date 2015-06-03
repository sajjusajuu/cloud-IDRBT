# IDRBT - Cloud Retrieval
# Webpage: https://www.idrbt.ac.in
# Contact: hussainsajid@acm.org
# Institute of Development and Research in Banking Technology
# 
# The Original Code is dictionary.py
# This program generates all the dictionary words from the files in list.txt
#
# Note: Required Pakages: NLTK from http://www.nltk.org/
# 
# Running : $ phthon dictionary.py list.txt
# Output : A sorted list containing all the words(important) in a JSON filed(dictionary.json)
#
# Contributor(s):
#  Sajid Hussain

import sys, re, json
import nltk, pprint
from nltk.corpus import wordnet

# stopwords is a set of most occuring english words like 'me', 'i', 'the' etc
stopwords = nltk.corpus.stopwords.words('english')
stopwords = set(stopwords)
alpahbets = 'abcdefghijklmnopqrstuvwxyz'
morestopwords = set(alpahbets)
stopwords = stopwords | morestopwords

def isword(s):
	for i in s:
		if not('a' <= i <= 'z'):
			return False
	return True



def make_dictionary(f, dictionary):
	# reading the complete file into a string
	s = f.read() 
	match = re.search(r'\r\n\r\n', s)
	# head contains the header of the mail
	head = s[: match.start()]
	# s contains the content of the mail
	s = s[match.start():]

	# patter for tokenization
	pattern = r'''(?x)
		([A-Z]\.)+
		| \w+(-\w+)*
		| \$?\d+(\.\d+)?%? 
		| \.\.\.
		| [][.,;"'?():-_`] 
	'''

	# tokens is a list containing all the words in the contents of the mail in list

	tokens = nltk.regexp_tokenize(s, pattern)
	tokens = [i.lower() for i in tokens]
	tokens = [i for i in tokens if isword(i)]

	wnl = nltk.WordNetLemmatizer()
	tokens = [wnl.lemmatize(t) for t in tokens]

	for word in tokens:
		if word not in stopwords:
			if wordnet.synsets(word):
				#print word
				dictionary.add(word)



if len(sys.argv) != 2:
	print """\
	Error in dictionary.py
	Usage: python dictionary.py list.txt
	"""
file_l = open(sys.argv[1], 'r') 

# The dictionary set object will hold all the tokens in the data set
dictionary = set()
count = 1
for line in file_l:							
	print 'processing', count , 'file'
	f = open(line.strip(), 'r')
	make_dictionary(f, dictionary)
	print 'dictionary has', len(dictionary), 'elements'
	count += 1
