import dotenv from "dotenv";
dotenv.config();
import { defineConfig } from "cypress";

export default defineConfig({
  e2e: {
    setupNodeEvents(on, config) {
      // implement node event listeners here
    },
  },
  env: { 
    CYPRESS_SERVER: process.env.CYPRESS_SERVER 
  }
}); 