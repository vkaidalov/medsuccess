<template>
  <div class="recipe-list">
    <recipe-detail
            class="recipe-detail"
            v-for="recipe in recipes"
            :key="recipe.id"
            :id="recipe.id"
            :date_created="recipe.date_created"
            :medicine="recipe.medicine"
            :reason="recipe.reason"
            :is_accepted="recipe.is_accepted"
            :doctor="recipe.doctor"
            :patient="recipe.patient"
    ></recipe-detail>
  </div>
</template>

<script>
  import RecipeDetail from "../components/RecipeDetail.vue";
  import axios from "axios";

  export default {
    name: "RecipeList",
    components: {
      RecipeDetail
    },
    data: function() {
      return {
        recipes: []
      }
    },
    mounted: function() {
      axios({url: 'http://localhost:8000/recipes/', data: {}, method: 'GET' })
        .then(resp => {
          this.recipes = resp.data;
          // resolve(resp);
        })
    }
  }
</script>

<style scoped>
.recipe-detail {
  padding: 10px;
  margin: 10px;
  border: 2px solid #1b6d85;
  border-radius: 5px;
}
</style>
