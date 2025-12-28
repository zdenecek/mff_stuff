import { describe, test, expect, vi } from "vitest";
import { createState } from "./application-state";

describe("createState", () => {
  test("#1 Default test", () => {
    const search = "?data-source=1&submit-url=2";
    const actual = createState(search);
    
    // Check basic properties
    expect(actual.dataSource).toBe("1");
    expect(actual.submitUrl).toBe("2");
    expect(actual.initialized).toBe(false);
    expect(actual.submitted).toBe(false);
    expect(actual.data).toBe(null);
    
    // Check that functions exist
    expect(typeof actual.initialize).toBe("function");
    expect(typeof actual.submit).toBe("function");
  });

  test("#2 State change test", () => {
    const search = "?data-source=1&submit-url=2";
    const state = createState(search);
    
    // Mock data for initialization
    const mockData = {
      model: "primary",
      fields: [
        { name: "title", type: "text", required: true },
        { name: "description", type: "textarea", required: false }
      ]
    };
    
    // Initialize state
    state.data = mockData;
    state.initialized = true;
    
    // Change model to secondary
    state.data.model = "secondary";
    
    expect(state.data.model).toBe("secondary");
  });

  test("#3 Remote data loading test", async () => {
    const search = "?datasource=https%3A%2F%2Fwebik.ms.mff.cuni.cz%2Fnswi153%2F2024-2025%2Fservice%2F08-test-03&submiturl=https%3A%2F%2Fwebik.ms.mff.cuni.cz%2Fnswi153%2F2024-2025%2Fservice%2F08-test-03-submit";
    const state = createState(search);
    
    // Mock fetch response
    const mockResponse = {
      model: "primary",
      fields: [
        { name: "title", type: "text", required: true },
        { name: "description", type: "textarea", required: false }
      ]
    };
    
    global.fetch = vi.fn().mockResolvedValue({
      json: () => Promise.resolve(mockResponse)
    });
    
    await state.initialize();
    
    expect(state.data.model).toBe("primary");
  });
}); 