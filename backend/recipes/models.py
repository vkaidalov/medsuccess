from django.db import models
from decimal import Decimal


class Recipe(models.Model):
    doctor = models.ForeignKey(
        'users.User',
        related_name='sent_recipes',
        on_delete=models.PROTECT,
    )
    patient = models.ForeignKey(
        'users.User',
        related_name='received_recipes',
        on_delete=models.PROTECT,
        null=True,
    )
    date_created = models.DateTimeField(auto_now_add=True)
    medicine = models.CharField(max_length=128)
    reason = models.TextField()
    is_accepted = models.BooleanField(null=True, default=None)
    min_temperature = \
        models.DecimalField(max_digits=6, decimal_places=2, default=Decimal('-10.0'))
    max_temperature = \
        models.DecimalField(max_digits=6, decimal_places=2, default=Decimal('25.0'))
    min_relative_humidity = \
        models.DecimalField(max_digits=6, decimal_places=2, default=Decimal('55.0'))
    max_relative_humidity = \
        models.DecimalField(max_digits=6, decimal_places=2, default=Decimal('65.0'))


class Dose(models.Model):
    recipe = models.ForeignKey(
        'recipes.Recipe',
        related_name='doses',
        on_delete=models.CASCADE
    )
    date_assigned = models.DateTimeField()
    is_consumed = models.BooleanField(default=False)
    quantity = models.DecimalField(max_digits=6, decimal_places=2)


class Measure(models.Model):
    recipe = models.ForeignKey(
        'recipes.Recipe',
        related_name='measures',
        on_delete=models.CASCADE
    )
    date_measured = models.DateTimeField()
    temperature = \
        models.DecimalField(max_digits=6, decimal_places=2)
    relative_humidity = \
        models.DecimalField(max_digits=6, decimal_places=2)
