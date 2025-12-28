export function createState(search) {
  const params = new URLSearchParams(search);
  const state = {
    dataSource: params.get('data-source') || params.get('datasource'),
    submitUrl: params.get('submit-url') || params.get('submiturl'),
    initialized: false,
    submitted: false,
    data: null
  };

  return {
    ...state,
    async initialize() {
      if (!this.dataSource) return;
      
      try {
        const response = await fetch(this.dataSource);
        this.data = await response.json();
        this.initialized = true;
      } catch (error) {
        console.error('Failed to initialize state:', error);
        this.initialized = false;
      }
    },
    
    async submit() {
      if (!this.submitUrl || !this.data) return;
      
      try {
        const response = await fetch(this.submitUrl, {
          method: 'POST',
          headers: {
            'Content-Type': 'application/json'
          },
          body: JSON.stringify(this.data)
        });
        
        if (response.ok) {
          this.submitted = true;
        }
      } catch (error) {
        console.error('Failed to submit data:', error);
      }
    }
  };
} 