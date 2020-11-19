from django.urls import path
from . import views

urlpatterns = [
    path('userprofile/', views.home, name = 'userProfile-home'),
    #path('about/', views.about, name = 'userProfile-about'),
    path('signup/', views.signup, name = 'userProfile-signup'),
	path('addwork/', views.addWorkingExperience, name = 'userProfile-addwork'),
	#path('addshippingaddress/', views.addshippingaddress, name = 'userProfile-addshippingaddress'),
	path('signin/', views.signin, name = 'userProfile-signin'),
	path('changepassword/', views.changepassword, name = 'userProfile-changepassword')
]
