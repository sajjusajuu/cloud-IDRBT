from django import forms

class SearchForm(forms.Form):
	file = 	forms.FileField()

class UploadForm(forms.Form):
	crypted_index_files = forms.FileField()
	crypted_docs = forms.FileField()