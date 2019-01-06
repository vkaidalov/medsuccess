from django.db import models


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
