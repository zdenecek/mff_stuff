import { defineStore } from 'pinia';
import { ref } from 'vue';
import { createState } from './application-state';

export const useApplicationState = defineStore('applicationState', () => {
  const state = ref(null);
  
  async function initialize(search = window.location.search) {
    state.value = createState(search);
    await state.value.initialize();
  }
  
  async function submit(data) {
    if (!state.value) return;
    
    state.value.data = data;
    await state.value.submit();
  }
  
  return {
    state,
    initialize,
    submit
  };
}); 