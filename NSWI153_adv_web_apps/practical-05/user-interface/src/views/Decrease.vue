<template>
  <v-card>
    <v-card-title>Decrease Counter</v-card-title>
    <v-card-text>
      <v-label for="decrease-value">
        {{ counter.count }}
      </v-label>
      <v-btn
        id="decrease-value"
        color="primary"
        size="large"
        @click="counter.decrease"
      >
        {{ counter.count }}
      </v-btn>

    </v-card-text>
  </v-card>
</template>

<script setup>
import { useCounterStore } from '../stores/counter'

const counter = useCounterStore()
</script> 