<template>
  <div>
    <form class="login" @submit.prevent="login">
      <h1>Sign in</h1>
      <label>Username</label>
      <input required v-model="username" type="text"/>
      <label>Password</label>
      <input required v-model="password" type="password"/>
      <hr/>
      <button type="submit">Login</button>
    </form>
  </div>
</template>

<script>
    export default {
        name: "Login",
        data() {
            return {
                username: "",
                password: ""
            }
        },
        methods: {
            login: function () {
                let username = this.username;
                let password = this.password;
                this.$store.dispatch('login', {username, password})
                    .then(() => this.$router.push('/'))
                    .catch(err => console.log(err));
            }
        }
    }
</script>

<style scoped>

</style>
