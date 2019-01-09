<template>
  <div>
    <p>ID: {{ id }}</p>
    <p>Date created: {{ created }}</p>
    <p>Medicine: {{ medicine }}</p>
    <p>Reason: {{ reason }}</p>
    <p>Is accepted: {{ is_accepted }}</p>
    <p>Doctor ID: {{ doctor }}</p>
    <p>Patient ID: {{ patient }}</p>
  </div>
</template>

<script>
  import dateFormat from "dateformat";

  export default {
    name: "RecipeDetail",
    props: {
      id: Number,
      date_created: String,
      medicine: String,
      reason: String,
      is_accepted: Boolean,
      doctor: Number,
      patient: Number
    },
    computed: {
      created: function() {
        return dateFormat(
          new Date(this.date_created),
          "dd.mm.yyyy HH:MM:ss"
        )
      }
    }
  }
</script>

<style scoped>

</style>
