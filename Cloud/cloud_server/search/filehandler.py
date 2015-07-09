
import os

def handlefile(f, name):
	with open(name, 'w') as destination:
		for chunk in f.chunks():
			destination.write(chunk)
	