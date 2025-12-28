describe('Dialog E2E Test', () => {
  it('#4 Dialog state change and submit', () => {
    // Mock the GET request
    cy.intercept('GET', '**/data-source.json', { fixture: 'data-source.json' }).as('getDataSource');

    // Mock the POST request
    cy.intercept('POST', '**/submit-url', (req) => {
      expect(req.body).to.deep.equal({
        model: 'secondary',
        title: 'Test Title',
        description: 'Test Description'
      });
      req.reply({ statusCode: 200 });
    }).as('submitData');

    // Visit the application with proper query parameters
    cy.visit(`${Cypress.env('CYPRESS_SERVER')}/dialog?data-source=data-source.json&submit-url=submit-url`);

    // Wait for the button to be visible and click it
    cy.get('[data-test="open-dialog-button"]').should('be.visible').click();

    // Wait for the dialog to be visible
    cy.get('.v-dialog').should('be.visible');

    // Wait for the data to be loaded
    cy.wait('@getDataSource');

    // Change model to secondary
    cy.get('[data-test="model-select"]').should('be.visible').click();
    cy.contains('secondary').should('be.visible').click();

    // Fill in the form fields
    cy.get('[data-test="text-field"]').should('be.visible').type('Test Title');
    cy.get('[data-test="textarea-field"]').should('be.visible').type('Test Description');

    // Click the create and clear button
    cy.get('[data-test="create-clear-button"]').should('be.visible').click();

    // Wait for the submit request
    cy.wait('@submitData');

    // Dialog should be closed
    cy.get('[data-test="open-dialog-button"]').should('be.visible');
  });
}); 