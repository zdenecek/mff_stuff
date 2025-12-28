import { createRouter, createWebHistory } from 'vue-router'
import Home from '../views/Home.vue'
import Increase from '../views/Increase.vue'
import Decrease from '../views/Decrease.vue'
import DialogView from '../views/DialogView.vue'

const baseUrl = import.meta.env.VITE_BASE_URL || '/'

const router = createRouter({
  history: createWebHistory(baseUrl),
  routes: [
    {
      path: '/',
      name: 'home',
      component: Home
    },
    {
      path: '/increase',
      name: 'increase',
      component: Increase
    },
    {
      path: '/decrease',
      name: 'decrease',
      component: Decrease
    },
    {
      path: '/dialog',
      name: 'dialog',
      component: DialogView
    }
  ]
})

export default router 