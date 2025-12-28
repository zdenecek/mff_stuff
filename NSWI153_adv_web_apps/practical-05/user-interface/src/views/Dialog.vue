<template>
  <v-card>
    <v-card-title>Create Association</v-card-title>
    <v-card-text>
      <v-form ref="form" v-model="valid">
        <v-select
          v-model="formData.model"
          :items="models"
          item-title="label"
          item-value="identifier"
          label="Model"
          :rules="[v => !!v || 'Model must be set.']"
        ></v-select>

        <v-select
          v-model="formData.specializations"
          :items="allAssociations"
          item-title="label"
          item-value="identifier"
          label="Specializations"
          multiple
          chips
        ></v-select>

        <v-select
          v-model="formData.profiles"
          :items="currentModelAssociations"
          item-title="label"
          item-value="identifier"
          label="Profiles"
          multiple
          chips
          return-object
          :rules="[v => Array.isArray(v) || 'Profiles must be an array']"
          @update:model-value="handleProfilesChange"
        ></v-select>

        <InheritableField
          v-model="formData.label"
          label="Label"
          :profiles="formData.profiles"
          v-model:inheritMode="formData.inheritLabel"
          v-model:inheritFrom="formData.labelInheritFrom"
          inherit-field="label"
        />

        <v-select
          v-model="formData.domain"
          :items="currentModelClasses"
          item-title="label"
          item-value="identifier"
          label="Domain"
          :rules="[v => !!v || 'Domain must be set.']"
          searchable
        ></v-select>

        <v-select
          v-model="formData.range"
          :items="currentModelClasses"
          item-title="label"
          item-value="identifier"
          label="Range"
          :rules="[v => !!v || 'Range must be set.']"
          searchable
        ></v-select>

        <v-select
          v-model="formData.rangeCardinality"
          :items="cardinalityOptions"
          label="Range cardinality"
        ></v-select>

        <InheritableField
          v-model="formData.description"
          label="Description"
          :profiles="formData.profiles"
          v-model:inheritMode="formData.inheritDescription"
          v-model:inheritFrom="formData.descriptionInheritFrom"
          inherit-field="description"
        />

        <v-btn
          color="primary"
          @click="submitForm"
          :disabled="!valid"
        >
          Create and clear
        </v-btn>
      </v-form>
    </v-card-text>
  </v-card>
</template>

<script setup>
import { ref, reactive, computed, onMounted, watch } from 'vue'
import { useRoute } from 'vue-router'
import InheritableField from '../components/InheritableField.vue'

const route = useRoute()
const form = ref(null)
const valid = ref(false)

const formData = reactive({
  model: null,
  specializations: [],
  profiles: [],
  label: '',
  inheritLabel: false,
  labelInheritFrom: null,
  description: '',
  inheritDescription: false,
  descriptionInheritFrom: null,
  domain: null,
  range: null,
  rangeCardinality: '0-*'
})

const models = ref([])
const allAssociations = ref([])
const allClasses = ref([])
const currentModelClasses = ref([])
const currentModelAssociations = ref([])

const cardinalityOptions = ['0-*', '1-*', '1-1', '*-*']

const handleProfilesChange = (newProfiles) => {
  if (!newProfiles.some(p => p.identifier === formData.labelInheritFrom)) {
    formData.labelInheritFrom = null
    formData.inheritLabel = false
  }
  if (!newProfiles.some(p => p.identifier === formData.descriptionInheritFrom)) {
    formData.descriptionInheritFrom = null
    formData.inheritDescription = false
  }
}

const updateModelData = (newModel) => {
  if (newModel) {
    console.debug('Current model:', newModel)
    console.debug('All associations:', allAssociations.value)
    currentModelClasses.value = allClasses.value.filter(c => c.model === newModel) || []
    currentModelAssociations.value = allAssociations.value.filter(a => a.model === newModel) || []
    console.debug('Filtered associations:', currentModelAssociations.value)
    // Reset dependent fields when model changes
    formData.profiles = []
    formData.domain = null
    formData.range = null
    formData.labelInheritFrom = null
    formData.descriptionInheritFrom = null
    formData.inheritLabel = false
    formData.inheritDescription = false
  }
}

const submitForm = async () => {
  const { valid } = await form.value.validate()
  if (!valid) {
    console.debug('Form is not valid' , form.value.value)
    return
  }

  const submitUrl = route.query['submit-url']
  if (!submitUrl) return

  try {
    await fetch(submitUrl, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json'
      },
      body: JSON.stringify(formData)
    })
    console.debug('Form submitted successfully')
    console.debug('Form data:', formData)
    // Reset form
    Object.assign(formData, {
      model: null,
      specializations: [],
      profiles: [],
      label: '',
      inheritLabel: false,
      labelInheritFrom: null,
      description: '',
      inheritDescription: false,
      descriptionInheritFrom: null,
      domain: null,
      range: null,
      rangeCardinality: '0-*'
    })
  } catch (error) {
    console.error('Error submitting form:', error)
  }
}

onMounted(async () => {
  const dataSource = route.query['data-source']
  if (!dataSource) return

  try {
    const response = await fetch(dataSource)
    const data = await response.json()
    models.value = data.models || []
    allAssociations.value = data.associations || []
    allClasses.value = data.classes || []
    
    // Set first model as default if available
    if (models.value.length > 0) {
      formData.model = models.value[0].identifier
      // Call updateModelData immediately after setting the initial model
      updateModelData(formData.model)
    }
    
    // Watch for model changes
    watch(() => formData.model, updateModelData)
  } catch (error) {
    console.error('Error loading data:', error)
  }
})
</script> 