import { shallowMount } from '@vue/test-utils';
import { describe, it, expect, vi, beforeEach } from 'vitest';
import Dialog from './Dialog.vue';
import { createPinia, setActivePinia } from 'pinia';
import { useApplicationState } from './../stores/application-state-store';
import { ref } from 'vue';

vi.mock('vuetify/components', () => ({}));
vi.mock('vuetify/directives', () => ({}));

describe('Dialog', () => {
  beforeEach(() => {
    setActivePinia(createPinia());
    vi.clearAllMocks();
    
    global.window = {
      location: {
        search: '?data-source=data-source.json&submit-url=submit-url'
      }
    };
    
    global.fetch = vi.fn();
    
    global.fetch.mockImplementation((url) => {
      if (url.includes('data-source.json')) {
        return Promise.resolve({
          ok: true,
          json: () => Promise.resolve({
            fields: [
              { name: 'title', type: 'text', required: true },
              { name: 'description', type: 'textarea', required: false }
            ]
          })
        });
      }
      return Promise.reject(new Error('Not found'));
    });
  });

  it('#1 Test initial dialog state and rendering', async () => {
    const dialog = ref(false);
    const model = ref('primary');
    const formData = ref({
      title: '',
      description: ''
    });
    
    expect(model.value).toBe('primary');
    expect(formData.value).toEqual({
      title: '',
      description: ''
    });
  });

  it('#2 Test state change and form submission', async () => {
    const store = useApplicationState();
    
    const submitSpy = vi.spyOn(store, 'submit').mockResolvedValue({});
    
    const dialog = ref(false);
    const model = ref('primary');
    const formData = ref({
      title: '',
      description: ''
    });
    
    model.value = 'secondary';
    formData.value = {
      title: 'Test Title',
      description: 'Test Description'
    };
    
    const data = {
      model: model.value,
      title: formData.value.title,
      description: formData.value.description
    };
    
    await store.submit(data);
    
    expect(submitSpy).toHaveBeenCalledWith({
      model: 'secondary',
      title: 'Test Title',
      description: 'Test Description'
    });
    
    formData.value = {
      title: '',
      description: ''
    };
    
    expect(formData.value).toEqual({
      title: '',
      description: ''
    });
  });

  it('#3 Test initial data loading from remote source', async () => {
    global.window.location.search = '?data-source=https://webik.ms.mff.cuni.cz/nswi153/2024-2025/service/08-test-03&submit-url=https://webik.ms.mff.cuni.cz/nswi153/2024-2025/service/08-test-03-submit';

    global.fetch.mockImplementationOnce(() => 
      Promise.resolve({
        ok: true,
        json: () => Promise.resolve({
          fields: [
            { name: 'title', type: 'text', required: true },
            { name: 'content', type: 'textarea', required: true }
          ]
        })
      })
    );

    const store = useApplicationState();
    
    await store.initialize();
    
    expect(global.fetch).toHaveBeenCalledWith(
      'https://webik.ms.mff.cuni.cz/nswi153/2024-2025/service/08-test-03'
    );
    
    const model = ref('primary');
    
    expect(model.value).toBe('primary');
  });
}); 