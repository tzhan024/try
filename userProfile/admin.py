from django.contrib import admin
from .models import Users
from .models import creditCards
from .models import shippingAddress
# Register your models here.

admin.site.register(Users)
admin.site.register(shippingAddress)
admin.site.register(creditCards)