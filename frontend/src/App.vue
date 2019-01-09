<template>
  <div id="app">
    <navigation/>
    <div class="content">
      <router-view/>
    </div>
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
  color: #2c3e50;
}

.content {
  width: 600px;
  margin: 0 auto;
}
</style>
