<template>
  <div>
    <h1>Recipe</h1>
    <p>ID: {{ recipe.id }}</p>
    <p>Date created: {{ recipe.created }}</p>
    <p>Medicine: {{ recipe.medicine }}</p>
    <p>Reason: {{ recipe.reason }}</p>
    <p>Is accepted: {{ recipe.is_accepted }}</p>
    <p>Doctor ID: {{ recipe.doctor }}</p>
    <p>Patient ID: {{ patient }}</p>
    <h1>Doses</h1>
    <ul>
      <li class="dose" v-for="dose in recipe.doses">
        <p>ID: {{ dose.id }}</p>
        <p>Date assigned: {{ dose.date_assigned }}</p>
        <p>Date consumed: {{ dose.date_consumed }}</p>
        <p>Quantity: {{ dose.quantity }}</p>
        <p>Recipe ID: {{ dose.recipe }}</p>
      </li>
    </ul>
  </div>
</template>

<script>
  import RecipeDetail from "../components/RecipeDetail";
  import axios from "axios";

  export default {
    name: "Recipe.vue",
    components: {
      RecipeDetail
    },
    data: function() {
      return {
        recipe: {}
      }
    },
    mounted: function() {
      console.log(this.$route.params.id);
      axios(
        {
          url: 'http://localhost:8000/recipes/' + this.$route.params.id + "/",
          data: {}, method: 'GET'
        }
          )
        .then(resp => {
          this.recipe = resp.data;
          // resolve(resp);
        })
    }
  }
</script>

<style scoped>
.dose {
  padding: 10px;
  margin: 10px;
  border: 2px solid #1b6d85;
  border-radius: 5px;
}
</style>
