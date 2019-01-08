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
      <!--TODO: add `v-if` directives-->
      <li>
        <router-link to="/register">Register</router-link>
      </li>
      <li>
        <router-link to="/login">Login</router-link>
      </li>
      <li @click="logout">
        <span class="logout">Logout</span>
      </li>
    </ul>
  </div>
</template>

<script>
  export default {
    name: "Navigation",
    methods: {
      logout: function() {
        // TODO: write the function body
        alert("I am logout!");
      }
    }
  }
</script>

<style lang="scss" scoped>
  a {
    color: white;
    text-decoration: none;
  }

  .navigation {
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
