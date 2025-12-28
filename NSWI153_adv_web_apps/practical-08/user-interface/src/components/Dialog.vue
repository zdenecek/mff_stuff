<template>
  <v-dialog v-model="dialog" max-width="600px" @update:model-value="onDialogUpdate">
    <template v-slot:activator="{ props }">
      <v-btn color="primary" v-bind="props" data-test="open-dialog-button">
        Open Dialog
      </v-btn>
    </template>

    <v-card>
      <v-card-title>
        <span class="text-h5">Dialog</span>
      </v-card-title>

      <v-card-text>
        <v-container>
          <v-row>
            <v-col cols="12">
              <v-select
                v-model="model"
                :items="['primary', 'secondary']"
                label="Model"
                data-test="model-select"
              ></v-select>
            </v-col>
            
            <v-col cols="12">
              <v-text-field
                v-model="formData.title"
                label="Title"
                required
                data-test="text-field"
              ></v-text-field>
            </v-col>
            
            <v-col cols="12">
              <v-textarea
                v-model="formData.description"
                label="Description"
                required
                data-test="textarea-field"
              ></v-textarea>
            </v-col>
          </v-row>
        </v-container>
      </v-card-text>

      <v-card-actions>
        <v-spacer></v-spacer>
        <v-btn
          color="blue-darken-1"
          variant="text"
          @click="dialog = false"
          data-test="close-button"
        >
          Close
        </v-btn>
        <v-btn
          color="blue-darken-1"
          variant="text"
          @click="createAndClear"
          data-test="create-clear-button"
        >
          Create and Clear
        </v-btn>
      </v-card-actions>
    </v-card>
  </v-dialog>
</template>

<script setup>
import { ref } from 'vue';
import { useApplicationState } from '../stores/application-state-store';

const dialog = ref(false);
const model = ref('primary');
const formData = ref({
  title: '',
  description: ''
});

const store = useApplicationState();

async function onDialogUpdate(value) {
  if (value) {
    await store.initialize();
  }
}

async function createAndClear() {
  const data = {
    model: model.value,
    title: formData.value.title,
    description: formData.value.description
  };
  
  await store.submit(data);
  formData.value = {
    title: '',
    description: ''
  };
  dialog.value = false;
}
</script> 