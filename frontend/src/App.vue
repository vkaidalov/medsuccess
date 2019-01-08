<template>
  <div id="app">
    <navigation/>
    <router-view/>
  </div>
</template>

<script>
  import Navigation from './components/Navigation'

  export default {
      components: {
          Navigation
      },
      name: 'app'
  }
</script>

<style>
  body {
    margin: 0;
  }

  #app {
    font-family: "Avenir", Helvetica, Arial, sans-serif;
    -webkit-font-smoothing: antialiased;
    -moz-osx-font-smoothing: grayscale;
    text-align: center;
    color: #2c3e50;
  }

</style>
