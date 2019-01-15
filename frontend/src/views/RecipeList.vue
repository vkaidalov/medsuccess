<template>
  <div class="recipe-list">
    <router-link class="create" to="recipes/create">
      <button>Create a new recipe</button>
    </router-link>
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
      axios({url: this.$store.state.hostname + '/recipes/', data: {}, method: 'GET' })
        .then(resp => {
          this.recipes = resp.data;
          // resolve(resp);
        })
    }
  }
</script>

<style scoped>
.create {
  background-color: #4CAF50; /* Green */
  border: none;
  color: white;
  padding: 15px 32px;
  text-align: center;
  text-decoration: none;
  display: inline-block;
  font-size: 16px;
}

.recipe-detail {
  padding: 10px;
  margin: 10px;
  border: 2px solid #1b6d85;
  border-radius: 5px;
}
</style>
