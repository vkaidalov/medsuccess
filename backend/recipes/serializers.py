from rest_framework import serializers
from recipes.models import Recipe


class RecipeSerializer(serializers.ModelSerializer):
    class Meta:
        model = Recipe
        fields = (
            'id', 'doctor', 'patient', 'date_created',
            'medicine', 'reason', 'is_accepted',
        )
