<template>
  <v-card>
    <v-card-title>Increase Counter</v-card-title>
    <v-card-text>
      <v-label for="increase-value">
        {{ counter.count }}
      </v-label>
      <v-btn
        id="increase-value"
        color="primary"
        size="large"
        @click="counter.increase"
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