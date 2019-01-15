<template>
  <div>
    <h1>Recipe</h1>
    <form class="editForm" @submit.prevent="editRecipe">
      <p>ID: {{ id }}</p>
      <p>Doctor ID: {{ doctor }}</p>

      <label for="patient">Patient ID:</label>
      <div>
        <input id="patient" type="text" v-model="patient" autofocus>
      </div>

      <p v-if="is_accepted != null">Is accepted by patient: {{ is_accepted }}</p>

      <label for="medicine">Medicine:</label>
      <div>
        <input id="medicine" type="text" v-model="medicine" required>
      </div>

      <label for="reason">Reason:</label>
      <div>
        <input id="reason" type="text" v-model="reason" required>
      </div>

      <h2>Storage</h2>

      <label for="mintemp">Minimal temperature:</label>
      <div>
        <input id="mintemp" type="text" v-model="min_temperature" required>
      </div>

      <label for="maxtemp">Maximal temperature:</label>
      <div>
        <input id="maxtemp" type="text" v-model="max_temperature" required>
      </div>

      <label for="minrh">Minimal RH:</label>
      <div>
        <input id="minrh" type="text" v-model="min_relative_humidity" required>
      </div>

      <label for="maxrh">Maximal RH:</label>
      <div>
        <input id="maxrh" type="text" v-model="max_relative_humidity" required>
      </div>

      <div>
        <button type="submit">{{ id ? "Save changes" : "Create a new recipe" }}</button>
      </div>
    </form>
    <h1>Doses</h1>
    <ul>
      <li class="dose" v-for="dose in doses">
        <p>ID: {{ dose.id }}</p>
        <p>Date assigned: {{ dose.date_assigned }}</p>
        <p>Date consumed: {{ dose.is_consumed }}</p>
        <p>Quantity: {{ dose.quantity }}</p>
      </li>
    </ul>
  </div>
</template>

<script>
  import axios from "axios";
  import store from "../store.js";

  export default {
    name: "Recipe.vue",
    data: function() {
      return {
        id: null,
        doctor: store.state.id,
        patient: null,
        medicine: "",
        reason: "",
        is_accepted: null,
        min_temperature: "-10.0",
        max_temperature: "25.0",
        min_relative_humidity: "55.0",
        max_relative_humidity: "55.0",
        doses: [],
        recipe: {}
      }
    },
    mounted: function() {
      console.log(this.$route.params.id);
      if (this.$route.params.id !== "create") {
        axios(
          {
            url: this.$store.state.hostname + '/recipes/' + this.$route.params.id + "/",
            data: {}, method: 'GET'
          }
        )
          .then(resp => {
            this.id = resp.data.id;
            this.doctor = resp.data.doctor;
            this.patient = resp.data.patient;
            this.medicine = resp.data.medicine;
            this.reason = resp.data.reason;
            this.is_accepted = resp.data.is_accepted;
            this.min_temperature = resp.data.min_temperature;
            this.max_temperature = resp.data.max_temperature;
            this.min_relative_humidity = resp.data.min_relative_humidity;
            this.max_relative_humidity = resp.data.max_relative_humidity;
            this.doses = resp.data.doses;
            // resolve(resp);
          })
      }
    },
    methods: {
      editRecipe: function() {
        let data = {
          doctor: this.doctor,
          patient: this.patient,
          medicine: this.medicine,
          reason: this.reason,
          min_temperature: this.min_temperature,
          max_temperature: this.max_temperature,
          min_relative_humidity: this.min_relative_humidity,
          max_relative_humidity: this.max_relative_humidity
        };

        console.log(this.doctor);

        if (this.id != null) {
          data.id = this.id;
          axios({url: store.state.hostname + '/recipes/' + data.id + '/', data: data, method: 'PATCH' })
          .then(resp => {
            alert("Changes are saved!");
            //resolve(resp);
          })
          .catch(err => {
            alert(err);
            //reject(err);
          })
        }
        else {
          axios({url: store.state.hostname + '/recipes/', data: data, method: 'POST' })
          .then(resp => {
            alert("Recipe is created!");
            //resolve(resp);
          })
          .catch(err => {
            console.log(err.response);
            alert(err);
            //reject(err);
          })
        }
      }
    }
  }
</script>

<style scoped>
.dose, .editForm {
  padding: 10px;
  margin: 10px;
  border: 2px solid #1b6d85;
  border-radius: 5px;
}

</style>
