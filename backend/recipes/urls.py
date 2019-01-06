from django.urls import path
from . import views

urlpatterns = [
    path(
        'recipes/',
        views.RecipeList.as_view(),
    ),
    path(
        'recipes/<int:pk>/',
        views.RecipeDetail.as_view(),
    ),
    path(
        'recipes/<int:fk>/doses/',
        views.DoseList.as_view(),
    )
]
