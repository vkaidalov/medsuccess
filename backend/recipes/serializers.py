from rest_framework import serializers
from recipes.models import Recipe, Dose


class DoseSerializer(serializers.ModelSerializer):
    class Meta:
        model = Dose
        fields = '__all__'


class RecipeSerializer(serializers.ModelSerializer):
    doses = DoseSerializer(many=True, read_only=True)

    class Meta:
        model = Recipe
        fields = '__all__'
