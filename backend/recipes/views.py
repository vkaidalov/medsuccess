from django.db.models import Q
from rest_framework.views import APIView
from rest_framework.response import Response
from rest_framework import status
from rest_framework import permissions
from recipes.models import Recipe
from recipes.serializers import RecipeSerializer


class RecipeList(APIView):
    """
    List all recipes, or create a new recipe.
    """
    permission_classes = (permissions.IsAuthenticated,)

    def get(self, request):
        recipes = Recipe.objects.filter(
            Q(doctor=request.user) | Q(patient=request.user)
        )
        serializer = RecipeSerializer(recipes, many=True)
        return Response(serializer.data)

    def post(self, request):
        serializer = RecipeSerializer(data=request.data)
        if serializer.is_valid():
            serializer.save()
            return Response(
                serializer.data,
                status=status.HTTP_201_CREATED,
            )
        return Response(
            serializer.errors,
            status=status.HTTP_400_BAD_REQUEST,
        )


# class RecipeDetail(APIView):
#     """
#     Retrieve a recipe instance.
#     Update or delete a recipe instance as a doctor
#     while `is_accepted` is not NULL.
#     Accept or don't accept a recipe instance as patient
#     while `is_accepted` is not NULL.
#     """
#
#