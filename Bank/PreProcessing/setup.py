# IDRBT - Cloud Retrieval
# Webpage: https://www.idrbt.ac.in
# Contact: hussainsajid@acm.org
# Institute of Development and Research in Banking Technology
# 
# The Original Code is setup.py
# 1. This program will generates all the dictionary words from the files from the given path
# 2. It will generate the keys depending upon the size of the dictionary
# 3. It will encrypt the original files and make them ready for upload
#
# Note: Required Pakages: NLTK from http://www.nltk.org/
# 
# Running : $ phthon setup.py <PATH>
# Output : 1. dictionary.json
# 		   2. The keys.
# 		   3. The encrypted databases
#
# Contributor(s):
#  Sajid Hussain





import sys, re, json, os, zipfile
import nltk, pprint, time, shutil

# The following will import the wordnet dictionary
from nltk.corpus import wordnet
from ctypes import *

# stopwords is a set of most occuring english words like 'me', 'i', 'the' etc

def generate_stop_words():
	stopwords = nltk.corpus.stopwords.words('english')
	stopwords = set(stopwords)
	alpahbets = u'abcdefghijklmnopqrstuvwxyz'
	r_words = [u'subject', u're', u'cc', u'fw']
	morestopwords = set(alpahbets) | set(r_words)
	stopwords = stopwords | morestopwords
	return stopwords



def isword(s):
	for i in s:
		if not('a' <= i <= 'z'):
			return False
	return True


def  get_words(f):
	# reading the complete file into a string
	s = f.read() 

	match_1 = re.search(r'Subject:', s)
	match_2 = re.search(r'Mime-Version:', s)

	s = s[match_1.start():match_2.start()]

	# patter for tokenization
	pattern = r'''(?x)
		([A-Z]\.)+
		| \w+(-\w+)*
		| \$?\d+(\.\d+)?%? 
		| \.\.\.
		| [][.,;"'?():-_`] 
	'''

	# tokens is a list, containing all the words in the contents of the mail
	tokens = nltk.regexp_tokenize(s, pattern)

	tokens = [i.lower() for i in tokens]
	tokens = tokens[1:] # removing term subject
	tokens = [i for i in tokens if isword(i)]

	word_set = set()
	for word in tokens:
		if word not in stopwords:
			word_set.add(word)

	return word_set


def make_index_vec(f):
	token_set = get_words(f)
	ind = []
	for i in dict_list:
		if i in token_set:
			ind.append(1)
		else:
			ind.append(0)
	return ind


if __name__ == "__main__":
	if len(sys.argv) != 2:
		print """\
		Error in setup.py
		Usage: python setup.py <PATH>
		Please give the path of the files to be indexed
		"""
		quit()	

	print """\n\n\
						       Welcome to
						THE CLOUD-IDRBT PROJECT
			"""

	PATH = sys.argv[1]
	if PATH[-1] != '/':
		PATH = PATH + '/'

	dictionary = set()
	# Improvement Needed to list files in different directories too...
	files = onlyfiles = [ f for f in os.listdir(PATH) if os.path.isfile(os.path.join(PATH,f))]
	num_files = len(files)
	print "Number of files found in this folder:", num_files
	#a = raw_input("\nDo you want to generate the dictionary set from these files? ")
	#if a == 'n' or a == 'N' or a == 'NO' or a == 'no' or a == 'No':
	#	quit()
	stopwords = generate_stop_words()
	#Remove the following comments to look through the stopwords
	pp = pprint.PrettyPrinter(indent=2)
	#pp.pprint(stopwords)

	count = 1
	for f in files:
		fp = open(os.path.join(PATH, f), 'r')
		word_set =  get_words(fp)
		dictionary = dictionary | word_set
		#time.sleep(0.05)
		sys.stdout.write("\rCreating the dictionary database: %d%% completed." % (count*100/num_files))
		sys.stdout.flush()
		fp.close()
		count += 1
	print "\n"
	num_words_dict = len(dictionary)
	print "Total number of words in the dictionary:", num_words_dict
	print "Storing the words in dictionary.json file:", 

	#This file is created in the same directory as this program
	dict_fp = open('dictionary.json', 'wr')
	dict_list = list(dictionary)
	dict_list.sort()
	json.dump(dict_list, dict_fp)
	dict_fp.close()
	print "Done"

	print "Creating Keys:"
	rand_seed = input("Enter a random number as a seed to generate the keys: ")
	current_path = os.getcwd()
	current_path = current_path[:-13]
	os.environ["LD_LIBRARY_PATH"] = os.path.join(current_path , "lib")
	key_lib = CDLL("generate_keys.so")
	crypt_lib = CDLL("crypting_vector.so")
	key_lib.generate_keys(c_int(num_words_dict), c_int(rand_seed))
	print "Keys Generation Completed."
	print "Creating the index files ..."
	os.makedirs("crypted_index_files")
	count = 1
	for f in files:
		fp = open(os.path.join(PATH, f), 'r')
		#print "opening file", os.path.join(PATH, f)
		#time.sleep(0.1)
		ind_vec = make_index_vec(fp)
		#print ind_vec
		ind_arr = (c_int * len(ind_vec))(*ind_vec)
		crypt_lib.crypt_vector(ind_arr, c_int(num_words_dict), c_int(0),  c_int(rand_seed))
		fp.close()
		dest = os.path.join("crypted_index_files", f)
		os.makedirs(dest)
		os.rename("crypt_1.key", os.path.join(dest, "crypt_1.key"))
		os.rename("crypt_2.key", os.path.join(dest, "crypt_2.key"))
		sys.stdout.write("\rCreating the index database: %d%% completed." % (count*100/num_files))
		sys.stdout.flush()
		count += 1
	print
	print "Zipping the Crypted Files ...", 
	crpzip = zipfile.ZipFile('crypted_index_files.zip', 'w')
	for root, dirs, fils in os.walk("crypted_index_files/"):
	    for file in fils:
	        crpzip.write(os.path.join(root, file))
	crpzip.close()
	print "Done"

	print("Encrypting the files...")
	os.makedirs('crypted_documents')
	passphrs = raw_input("Enter the passphrase for encrypting the documents: ")
	count = 1
	for f in files:
		fp = os.path.join(PATH, f)
		#time.sleep(0.1)
		os.system('echo ' + passphrs + ' | gpg --output /home/sajid/cloud-IDRBT/Bank/PreProcessing/crypted_documents/' + f + ' --symmetric --cipher-algo aes256 --passphrase-fd 0 --batch ' +  fp)
		sys.stdout.write("\rCreating the Crypted documents database: %d%% completed." % (count*100/num_files))
		sys.stdout.flush()
		count += 1
	print
	print "Zipping the Crypted Documents ...", 
	crpzip = zipfile.ZipFile('crypted_documents.zip', 'w')
	for root, dirs, fils in os.walk("crypted_documents/"):
	    for file in fils:
	        crpzip.write(os.path.join(root, file))
	crpzip.close()
	print "Done"
	print "Configuring the server ... ",
	server_path = os.path.join(current_path, "Server")
	shutil.copy("s_i.key", server_path)
	shutil.copy("m1_i.key", server_path)
	shutil.copy("m2_i.key", server_path)
	shutil.copy("dictionary.json", server_path)
	print "Done"