from django.shortcuts import render, redirect
from django.http import HttpResponse
from django.contrib.auth.models import User, auth
from django.contrib import messages
from .models import Users
from .models import creditCards
from .models import workExperience
from django.shortcuts import get_object_or_404
import datetime
#from django.contrib.auth.hashers import make_password
#currentUser = "123"

def home(request): 
    #return HttpResponse('<h1>userProfile userProfile</h1>')
	#currentUser = signin(request)
	#print("un: " + currentUser + "\n")
	#if request.user.is_authenticated:
		#name = request.user.username
	users = Users.objects.filter(Username = request.user.username)
	cards = creditCards.objects.filter(Username = request.user.username)
	works = workExperience.objects.filter(Username = request.user.username)
	if request.method == 'POST':
		fullname = request.POST['name']
		nickname = request.POST['nickname']
		email = request.POST['email']
		phone = request.POST['phone']
		address = request.POST['address']

		thisuser = Users.objects.filter(Username = request.user.username).update(name = fullname, nickName = nickname, Email = email, phoneNumber = phone, Address = address)
	
	#if request.method == 'POST':
	#	address = request.POST['address']
	#	city = request.POST['city']
	#	state = request.POST['state']
	#	zipcode = reqiest.POST['zipcode']
	#	
	#if request.method == 'POST' && request.action == 'changeEmail':
		
	#if request.method == 'POST' && request.action == 'editProfile':
		
	#if request.method == 'POST' && request.action == 'editProfile':
		
	
	#if request.method == 'POST' && request.action == 'changePassword':
		
	return render (request, 'userProfile/Profiletemplete.html', {'users': users, 'cards': cards, 'works': works})

def changepassword(request):
	if request.method == 'POST':
		oldpassword = request.POST['oldpass']
		newpass1 = request.POST['newpass1']
		newpass2 = request.POST['newpass2']
		
		if request.user.is_authenticated:
			thisuser = User.objects.get(username = request.user.username, password = request.user.password)
			#print(thisuser.username + "   " + thisuser.password)
			if thisuser is not None:
				print('user exists')
				if newpass1 == newpass2:
					if len(newpass2) >= 6:
						if any(c.isalpha() for c in newpass2):
							#print('password matches')
							#thisuser.set_password(newpass2)
							thisuser.set_password(newpass2)
							thisuser.save()
							return redirect('/signin')
						else:
							messages.info(request, 'password must contain letter')
							return redirect('/changepassword')
					else:
						messages.info(request, 'password too short')
						return redirect('/changepassword')
				else:
					messages.info(request, 'confirm password not match')
					return redirect('/changepassword')
			else:
				messages.info(request, 'password incorrect')
				return redirect('/changepassword')
	else: 
		return render(request, 'userProfile/changepassword.html')

'''def addcreditcard(request):
	if request.method == 'POST': 
		fullname = request.POST['name']
		card_number = request.POST['cardnumber']
		expDate = request.POST['expdate']
		cardType = request.POST['cardtype']
		cvv = request.POST['cvv']
		date = datetime.datetime.strptime(expDate, "%m/%y").date()
		if request.user.is_authenticated:
			if date >= datetime.datetime.now().date():
				print(date)
				name = request.user.username
				card = creditCards(name = fullname, card_number = card_number, expDate = expDate, cardType = cardType, cvv = cvv, Username = name)
				card.save()
				return redirect('/userprofile')
			else:
				messages.info(request, 'Expiration date invalid.')
				return redirect('/addcreditcard')
	else: 
		return render (request, 'userProfile/addcard.html')'''
	
def addWorkingExperience(request): 
	if request.method == 'POST': 
		job = request.POST['job']
		years = request.POST['years']
		company = request.POST['company']
		comment = request.POST['comment']
		if request.user.is_authenticated:
			name = request.user.username
			works = workExperience(job = job, years = years, company = company, comment = comment, Username = name)
			works.save()
			return redirect('/userprofile')
	else: 
		return render (request, 'userProfile/addwork.html')
	
'''def addshippingaddress(request): 
	if request.method == 'POST': 
		address = request.POST['address']
		city = request.POST['city']
		state = request.POST['state']
		zipcode = request.POST['zipcode']
		if request.user.is_authenticated:
			name = request.user.username
			address = shippingAddress(address = address, city = city, state = state, zipCode = zipcode, Username = name)
			address.save()
			return redirect('/userprofile')
	else: 
		return render (request, 'userProfile/addaddress.html')'''

def about(request):
	return HttpResponse('<h1>userProfile about</h1>')
	
def signup(request):
	if request.method == 'POST': 
		username = request.POST['username']
		password = request.POST['password']
		name = request.POST['name']
		nickname = request.POST['nickname']
		email = request.POST['email']
		phone = request.POST['phone']
		address = request.POST['address']
		if User.objects.filter(username = username).exists():
			print('username taken')
			messages.info(request, 'username taken')
			return redirect('/signup')
		else: 
			if len(password) >= 6:
				if any(c.isalpha() for c in password):
					authuser = User.objects.create_user(username = username, password = password, email = email)
					authuser.save()
					user = Users(Username = username, name = name, nickName = nickname, Email = email, phoneNumber = phone, Address = address)
					user.save()
					return redirect('/signin')
				else:
					messages.info(request, 'password must contain letter')
					return redirect('/signup')
			else:
				messages.info(request, 'password too short')
				return redirect('/signup')
	else:
		return render (request, 'userProfile/signup.html')

def signin(request):
	if request.method == 'POST': 
		un = request.POST['username']
		pw = request.POST['password']
		authuser = auth.authenticate(username = un, password = pw)
		
		if authuser is not None: 
			auth.login(request, authuser)
			return redirect('/userprofile')
			
		else:
			messages.info(request, 'password or username invalid')
			return redirect('/signin')
	else:
		return render (request, 'userProfile/signin.html')