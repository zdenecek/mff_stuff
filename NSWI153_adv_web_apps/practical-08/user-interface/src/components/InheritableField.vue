<template>
  <div>
    <v-text-field
      v-model="localValue"
      :label="label"
      :readonly="isInheritMode"
      :rules="[v => !isInheritMode || !!v || `${label} must not be empty.`]"
    ></v-text-field>

    <v-checkbox
      v-if="profiles.length > 0"
      v-model="isInheritMode"
      :label="`Inherit ${label.toLowerCase()}`"
      @update:model-value="handleInheritModeChange"
    ></v-checkbox>

    <v-select
      v-if="isInheritMode && profiles.length > 0"
      v-model="selectedProfile"
      :items="profiles"
      item-title="label"
      item-value="identifier"
      :label="`${label} inherit from`"
      return-object
      @update:model-value="handleProfileChange"
    ></v-select>
  </div>
</template>

<script setup>
import { ref, computed, watch } from 'vue'

const props = defineProps({
  modelValue: {
    type: String,
    default: ''
  },
  label: {
    type: String,
    required: true
  },
  profiles: {
    type: Array,
    default: () => []
  },
  inheritMode: {
    type: Boolean,
    default: false
  },
  inheritFrom: {
    type: String,
    default: null
  },
  inheritField: {
    type: String,
    required: true
  }
})

const emit = defineEmits(['update:modelValue', 'update:inheritMode', 'update:inheritFrom'])

const localValue = ref(props.modelValue)
const isInheritMode = computed({
  get: () => props.inheritMode,
  set: (value) => emit('update:inheritMode', value)
})
const selectedProfile = computed({
  get: () => props.profiles.find(p => p.identifier === props.inheritFrom),
  set: (value) => emit('update:inheritFrom', value?.identifier)
})

// Keep track of manual value when switching modes
const manualValue = ref(props.modelValue)

// Update local value when model value changes
watch(() => props.modelValue, (newValue) => {
  if (!isInheritMode.value) {
    localValue.value = newValue
    manualValue.value = newValue
  }
})

// Handle inherit mode changes
const handleInheritModeChange = (value) => {
  if (value) {
    // Switching to inherit mode - store current value as manual value
    manualValue.value = localValue.value
    // Set value from selected profile if exists
    if (selectedProfile.value) {
      localValue.value = selectedProfile.value[props.inheritField]
    }
  } else {
    // Switching back to manual mode - restore manual value
    localValue.value = manualValue.value
  }
}

// Handle profile selection changes
const handleProfileChange = (value) => {
  if (value) {
    localValue.value = value[props.inheritField]
  }
}

// Watch for profile changes to handle removal of selected profile
watch(() => props.profiles, (newProfiles) => {
  if (isInheritMode.value && props.inheritFrom) {
    const profileExists = newProfiles.some(p => p.identifier === props.inheritFrom)
    if (!profileExists) {
      // Selected profile was removed - switch back to manual mode
      isInheritMode.value = false
      emit('update:inheritFrom', null)
      localValue.value = manualValue.value
    }
  }
}, { deep: true })

// Watch for changes in the selected profile's value
watch(() => selectedProfile.value?.[props.inheritField], (newValue) => {
  if (isInheritMode.value && newValue !== undefined) {
    localValue.value = newValue
  }
})
</script> 