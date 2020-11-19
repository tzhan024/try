from django.db import models

# Create your models here.
class Users (models.Model): 
	Email = models.CharField(max_length = 40)
	Username = models.CharField(max_length = 20)
	name = models.CharField(max_length = 40)
	Address = models.CharField(max_length = 40)
	phoneNumber = models.CharField(max_length = 10)
	nickName = models.CharField(max_length = 20)
	password = models.CharField(max_length = 40)
	
class creditCards (models.Model):
	name = models.CharField(max_length = 40)
	card_number = models.CharField(max_length = 20)
	expDate = models.CharField(max_length = 5)
	cardType = models.CharField(max_length = 15)
	cvv = models.CharField(max_length = 3)
	Username = models.CharField(max_length = 20)
	
class shippingAddress (models.Model):
	address = models.CharField(max_length = 40)
	city = models.CharField(max_length = 20)
	state = models.CharField(max_length = 2)
	zipCode = models.CharField(max_length = 5)
	Username = models.CharField(max_length = 20)
	
class workExperience (models.Model):
	job = models.CharField(max_length = 40)
	years = models.CharField(max_length = 20)
	company = models.CharField(max_length = 30)
	comment = models.CharField(max_length = 150)
	Username = models.CharField(max_length = 20)

    