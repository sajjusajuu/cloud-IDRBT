from django.db import models

class User(models.Model):
	user_name = models.CharField(max_length = 10, primary_key = True)
	pass_word = models.CharField(max_length = 20)
	def __str__(self):
		return self.user_name

