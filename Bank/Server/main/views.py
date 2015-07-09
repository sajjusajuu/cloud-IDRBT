from django.shortcuts import render

from django.http import HttpResponse

from .models import User
from django.core.urlresolvers import reverse
from django.core.servers.basehttp import FileWrapper

import sys, os
#sys.path.insert(0, '/home/sajid/cloud-IDRBT/Bank/Server')

import q_vector


def index(request):
    return render(request, 'main/login.html')


def login(request):
	try:
		p_w = User.objects.get(user_name = request.POST['username'])
		if p_w.pass_word == request.POST['password']:
			return render(request, 'main/search.html', {'us' : p_w.user_name})
		else:
			return render(request, 'main/login.html')
	except :
		return render(request, 'main/login.html')

def search(request):
	k_string = request.POST['keywords']
	
	q_vector.gen_query(k_string)
	#This directory will have a query.zip file that it need to send it to the client
	response = HttpResponse(open('query.zip', 'rb').read(), content_type='application/zip')
	response['Content-Disposition'] = 'attachment; filename=query.zip'
	os.remove('query.zip')
	return response




