from django.utils.dateparse import parse_datetime
from django.db.models import Q
from django.http import Http404
from rest_framework.views import APIView
from rest_framework.response import Response
from rest_framework import status
from rest_framework import permissions
from recipes.models import Recipe, Dose
from recipes.serializers import RecipeSerializer, DoseSerializer


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
        # TODO: Restrict a doctor to specify the `is_accepted` field.
        try:
            if request.data['doctor'] != request.user.id:
                return Response(
                    {
                        "doctor": ["Only the creator of the recipe can be the doctor."]
                    },
                    status=status.HTTP_400_BAD_REQUEST,
                )
        except KeyError:
            return Response(
                {
                    "doctor": ["This field is required."]
                },
                status=status.HTTP_400_BAD_REQUEST,
            )

        serializer = RecipeSerializer(data=request.data)

        if not serializer.is_valid():
            return Response(
                serializer.errors,
                status=status.HTTP_400_BAD_REQUEST,
            )

        serializer.save()
        return Response(
            serializer.data,
            status=status.HTTP_201_CREATED,
        )


class RecipeDetail(APIView):
    """
    Retrieve a recipe instance.
    Update or delete a recipe instance as a doctor
    while `is_accepted` is not NULL.
    Accept or don't accept a recipe instance as patient
    while `is_accepted` is not NULL.
    """
    permission_classes = (permissions.IsAuthenticated,)

    def get_object(self, pk):
        try:
            return Recipe.objects.get(pk=pk)
        except Recipe.DoesNotExist:
            raise Http404

    def get(self, request, pk):
        recipe = self.get_object(pk)

        if recipe.doctor != request.user and recipe.patient != request.user:
            raise Http404

        serializer = RecipeSerializer(recipe)
        return Response(serializer.data)

    def put(self, request, pk):
        """
        This method works only for patients.
        A patient can accept or decline a recipe if `is_accepted` equals NULL.
        """
        recipe = self.get_object(pk)

        if recipe.patient != request.user:
            return Response(
                status=status.HTTP_404_NOT_FOUND,
            )

        if recipe.is_accepted is not None:
            return Response(
                {
                    "is_accepted": ["The recipe is already accepted or declined."]
                },
                status=status.HTTP_400_BAD_REQUEST,
            )

        try:
            is_accepted = request.data['is_accepted']
        except KeyError:
            return Response(
                {
                    "is_accepted": ["The field is required."]
                },
                status=status.HTTP_400_BAD_REQUEST
            )

        if type(is_accepted) != bool:
            return Response(
                {
                    "is_accepted": ["The field must be true/false."]
                },
                status=status.HTTP_400_BAD_REQUEST,
            )

        recipe.is_accepted = is_accepted
        recipe.save()

        serializer = RecipeSerializer(recipe)
        return Response(
            serializer.data,
            status=status.HTTP_200_OK
        )

    def patch(self, request, pk):
        # TODO: Restrict a doctor to modify the `is_accepted` field.
        """
        This method works only for doctors.
        A doctor can modify fields and assign
        a patient to the recipe to be accepted/declined.
        """
        recipe = self.get_object(pk)

        if recipe.doctor != request.user:
            return Response(
                status=status.HTTP_404_NOT_FOUND,
            )

        if recipe.is_accepted is not None:
            return Response(
                {
                    "is_accepted": ["Can't modify the accepted/declined recipe."]
                },
                status=status.HTTP_400_BAD_REQUEST,
            )

        serializer = RecipeSerializer(recipe, data=request.data)

        if not serializer.is_valid():
            return Response(
                serializer.errors,
                status=status.HTTP_400_BAD_REQUEST,
            )

        serializer.save()
        return Response(serializer.data)

    def delete(self, request, pk):
        recipe = self.get_object(pk)

        if recipe.doctor != request.user:
            return Response(
                status=status.HTTP_404_NOT_FOUND,
            )

        if recipe.is_accepted is not None:
            return Response(
                {
                    "is_accepted": ["Can't delete the accepted/declined recipe."]
                },
                status=status.HTTP_400_BAD_REQUEST,
            )

        recipe.delete()

        return Response(
            status=status.HTTP_204_NO_CONTENT,
        )


class DoseList(APIView):
    """
    Create a new dose for a recipe as a doctor.
    """

    permission_classes = (permissions.IsAuthenticated,)

    def get_recipe(self, pk):
        try:
            return Recipe.objects.get(pk=pk)
        except Recipe.DoesNotExist:
            raise Http404

    def post(self, request, fk):
        """
        The method allows doctors to add a new dose to a recipe.
        """
        # TODO: check if fk in request.data is the same with <int:fk>
        # TODO: check if `date_consumed` is None
        recipe = self.get_recipe(fk)

        if recipe.doctor != request.user:
            return Response(
                status=status.HTTP_404_NOT_FOUND,
            )

        if recipe.is_accepted is not None:
            return Response(
                {
                    "is_accepted": ["Can't add a dose to the accepted/declined recipe."]
                },
                status=status.HTTP_400_BAD_REQUEST,
            )

        dose_serializer = DoseSerializer(data=request.data)

        if not dose_serializer.is_valid():
            return Response(
                dose_serializer.errors,
                status=status.HTTP_400_BAD_REQUEST,
            )

        dose_serializer.save()
        return Response(dose_serializer.data, status=status.HTTP_201_CREATED)


class DoseDetail(APIView):
    """
    Delete a dose from a recipe as a doctor.
    Write the `date_consumed` field of a dose as a patient.
    """
    def get_recipe(self, pk):
        try:
            return Recipe.objects.get(pk=pk)
        except Recipe.DoesNotExist:
            raise Http404

    def get_dose(self, pk):
        try:
            return Dose.objects.get(pk=pk)
        except Dose.DoesNotExist:
            raise Http404

    def delete(self, request, fk, pk):
        recipe = self.get_recipe(fk)

        if recipe.doctor != request.user:
            raise Http404

        if recipe.is_accepted is not None:
            return Response(
                {
                    "is_accepted": ["Can't add a dose to the accepted/declined recipe."]
                },
                status=status.HTTP_400_BAD_REQUEST,
            )

        dose = self.get_dose(pk)

        if dose.recipe != recipe:
            raise Http404

        dose.delete()
        return Response(
            status=status.HTTP_204_NO_CONTENT,
        )

    def put(self, request, fk, pk):
        # TODO: Check if `date_consumed` >= `date_assigned`
        recipe = self.get_recipe(fk)

        if recipe.patient != request.user:
            raise Http404

        if recipe.is_accepted is None:
            return Response(
                {
                    "is_accepted": ["Can't consume a dose which isn't in an accepted/declined recipe."]
                },
                status=status.HTTP_400_BAD_REQUEST,
            )

        dose = self.get_dose(pk)

        if dose.recipe != recipe:
            raise Http404

        try:
            datetime = parse_datetime(request.data["date_consumed"])
        except KeyError:
            return Response(
                {
                    "date_consumed": ["The field is required."]
                },
                status=status.HTTP_400_BAD_REQUEST,
            )
        except ValueError:
            return Response(
                {
                    "date_consumed": ["The field isn't a valid datetime."]
                },
                status=status.HTTP_400_BAD_REQUEST,
            )

        if datetime is None:
            return Response(
                {
                    "date_consumed": ["The field isn't well formatted."]
                },
                status=status.HTTP_400_BAD_REQUEST,
            )

        dose.date_consumed = datetime
        dose.save()
        serializer = DoseSerializer(dose)
        return Response(serializer.data)

