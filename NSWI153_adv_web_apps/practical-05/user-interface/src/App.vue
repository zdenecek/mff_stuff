<script setup>
import HelloWorld from './components/HelloWorld.vue'

const navItems = [
  { name: 'Home', path: '/', target: 'home' },
  { name: 'Increase', path: '/increase', target: 'increase' },
  { name: 'Decrease', path: '/decrease', target: 'decrease' },
  { name: 'Dialog', path: '/dialog', target: 'dialog' }
]
</script>

<template>
  <v-app>
    <v-app-bar>
      <v-container class="d-flex align-center">
        <v-btn
          v-for="item in navItems"
          :key="item.path"
          :to="item.path"
          :data-target="item.target"
          variant="text"
        >
          {{ item.name }}
        </v-btn>
      </v-container>
    </v-app-bar>

    <v-main>
      <v-container>
        <router-view></router-view>
      </v-container>
    </v-main>
  </v-app>
</template>

<style scoped>
.logo {
  height: 6em;
  padding: 1.5em;
  will-change: filter;
  transition: filter 300ms;
}
.logo:hover {
  filter: drop-shadow(0 0 2em #646cffaa);
}
.logo.vue:hover {
  filter: drop-shadow(0 0 2em #42b883aa);
}
</style>
