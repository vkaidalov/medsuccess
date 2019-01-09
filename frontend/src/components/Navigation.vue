<template>
  <div class="navigation">
    <ul>
      <li>
        <router-link class="brand" to="/">
          <strong>MedSuccess</strong>
        </router-link>
      </li>
    </ul>
    <ul>
      <li v-if="isLoggedIn">
        <router-link to="/recipes">Recipes</router-link>
      </li>
      <li v-if="!isLoggedIn">
        <router-link to="/register">Register</router-link>
      </li>
      <li v-if="!isLoggedIn">
        <router-link to="/login">Login</router-link>
      </li>
      <li v-if="isLoggedIn" @click="logout">
        <span class="logout">Logout</span>
      </li>
    </ul>
  </div>
</template>

<script>
  import { mapGetters } from 'vuex'

  export default {
    name: "Navigation",
    methods: {
      logout: function() {
        this.$store.dispatch('logout')
          .then(() => {
            this.$router.push('/')
          })
      }
    },
    computed: {
        ...mapGetters(['isLoggedIn'])
    }
  }
</script>

<style lang="scss" scoped>
  a {
    color: white;
    text-decoration: none;
  }

  .navigation {
    font-size: 20px;
    display: flex;
    color: white;
    align-items: center;
    background-color: #006951;
    padding: 5px;

    ul {
      list-style-type: none;
      display: flex;
      &:first-child {
        flex-grow: 1;
      }
      li {
        padding-right: 1em;
      }
    }
  }

  .brand {
    display: flex;
    align-items: center;

  }

  .logout {
    &:hover {
      cursor: pointer;
    }
  }

</style>
