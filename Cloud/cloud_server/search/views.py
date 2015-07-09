from django.shortcuts import render
from django.http import HttpResponse

from .forms import SearchForm, UploadForm
from .filehandler import handlefile
from operator import itemgetter
import os, zipfile
from ctypes import *

def index(request):
	form = SearchForm()
	try:
		num_files = len([name for name in os.listdir('crypted_index_files')])
	except:
		num_files = 0
	return render(request, 'search/search.html', {'form': form , 'num_files' : num_files})

def uploadreq(request):
	form = UploadForm()
	return render(request, 'search/uploadreq.html', {'form' : form})



def upload(request):
	form = SearchForm(request.POST, request.FILES)

	handlefile(request.FILES['crypted_index_files'], 'crypted_index_files.zip')
	handlefile(request.FILES['crypted_docs'], 'crypted_documents.zip')

	zfile = zipfile.ZipFile('crypted_index_files.zip')
	zfile.extractall()
	zfile = zipfile.ZipFile('crypted_documents.zip')
	zfile.extractall()

	return index(request)

def files(request):
	f_list = [name for name in os.listdir('crypted_index_files')]
	f_list.sort()
	f_list = sorted(f_list, key=lambda ele: len(ele))
	return render(request, 'search/list.html', {'f_list' : f_list})

def down(request, number):
	add_f = os.path.join('crypted_documents', number + '.')
	response = HttpResponse(open(add_f, 'rb').read(), content_type='text/plain')
	response['Content-Disposition'] = 'attachment; filename=' + number + '.'
	return response


def search(request):
	form = SearchForm(request.POST, request.FILES)

	#This file handler will unzip the received file and the trapdoor files are in a folder 'temp'.
	handlefile(request.FILES['file'], 'temp.zip')

	zfile = zipfile.ZipFile('temp.zip')
	zfile.extractall()
	os.system("rm temp.zip")

	
	add = 'crypted_index_files'

	c_direc = os.getcwd()
	lib_direc = os.path.join(c_direc[:-12], "lib")
	mult_lib = CDLL(os.path.join(lib_direc, "multiply.so"))

	list_d  = os.listdir(add)
	l = []
	for i in list_d:
		#print "matching", i, "file"
		c_dir = os.path.join(add, i)
		c_f_1 = c_char_p(os.path.join(c_dir, "crypt_1.key"))
		c_f_2 = c_char_p(os.path.join(c_dir, "crypt_2.key"))
		score = mult_lib.multiply(c_f_1, c_f_2, c_int(725))
		#print 'This is score ', score
		l.append((i, score))

	os.remove("crypt_1.key")
	os.remove("crypt_2.key")
	l = sorted(l, key=lambda lis: lis[1], reverse=True)
	top_c = 0
	for i in l:
		#print i[0], i[1]
		if i[1] > 0:
			top_c += 1

	if top_c == 0:
		top_c = 1
	elif top_c > 5:
		top_c = 5

	f_list = [l[i][0] for i in range(0, top_c)]
	return render(request, 'search/list.html', {'f_list' : f_list})

