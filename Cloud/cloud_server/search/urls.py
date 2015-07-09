from django.conf.urls import url

from . import views

urlpatterns = [
	url(r'^search', views.search, name = 'search'),
	url(r'^upldr', views.uploadreq, name = 'upldr'),
	url(r'^upload', views.upload, name = 'upload'),
	url(r'^files', views.files, name = 'files'),
	url(r'^down/(?P<number>[0-9]+).$', views.down, name = 'down'),
	url(r'^$', views.index, name = 'index'),
	
]