Advanced
Web Applications
JavaScript, User Interface
Objectives
• User Interface
This is an DIY practical.
You must to get your solution to your GitLab repository, and deploy it to webik, before 6.4.2025.
Assignment: User Interface
./practical-05/user-interface
webik.ms.mff.cuni.cz/~{your-user-name}/nswi153/practical-05/
Employ Vite and rendering library ( , , , , ... ) of choice to implement
functionality as described at following slides.
package.json must contain the build script for building the application into dist directory.
React Vue Angular Svelte
Create a simple application
Create a simple application with pages: Home, Increase, Decrease, Dialog.
Each page must have a navigation bar with links to all pages in your application. The application
must not reload upon navigation (single-page-application style). You may implement routing
(navigating from page to page) yourself or utilize a library.
Use following URLs:
• Home - '/'
• Increase - 'increase'
• Decrease - 'decrease'
• Dialog - 'dialog'
Home page
Content of your choice.
Shared state
Pages 'Increase' and 'Decrease' should contain a button. The label of the button should be a value
of a counter. The counter must be shared between pages. When a user clicks the first button
('Increase' page), with id = "increase-value", the counter will increase. When the user clicks the
other button ('Decrease' page), with id = "decrease-value", the counter will decrease. Keep in mind
that each button is located on a different page.
Your framework of choice may provide a state management library or technique (context) to help
you with sharing the data.
Utilize a UI library
Find and use any ui component library compatible with your selected framework. Add layout to
the application, and make it look cool.
Below are libraries you may consider. Keep in mind that libraries are often framework specific.
•
•
•
MUI
Reactstrap
Vuetify
Navigation bar
The navigation bar must contain links to all pages. In the navigation bar each "a" element must
contain property "data-target" with value according to following list:
• Home - 'home'
• Increase - 'increase'
• Decrease - 'decrease'
• Dialog - 'dialog'
Dialog
Purpose of this page is to create a non-trivial dialog. The dialog should allow user create an
. The association can be placed inside a model, can be specialization of another
association or can inherit values from other associations. We need to be able to edit following:
• Model - the model we want to create the association into. The input property should allow user to
select from available models. By default the first model is selected.
• Specializations - the user can select none, one, or more associations to specialize. Use can select
values from all available associations. By default no value is selected.
• Profiles - the user can select none, one, or more associations to profile. User can only select from
associations that are in the currently selected Model. When user change Model all selected profiles
must be removed. By default no value is selected.
See next slide for more fields.
association
Dialog
The dialog should allow user to inherit values from a selected profile. The idea is that the user can
either provide value manually (manual-mode) or inherit the value (inherit-mode). The "inheritmode"
is active when Inherit label is checked an a profile is selected in Label inherit from.
• Label - a text input for a label. When in the "manual-mode" the user can enter a value directly.
When in the "inherit-mode" the input is in a read only mode, showing the inherited value. The
"manual-mode" value must not be lost by switching to the "inherit-mode" and back.
• Inherit label - a check box indicating whether label should be inherited or not. The Inherit label is
visible only when Profiles is not empty.
• Label inherit from - the user can select a profile from Profiles. The Label inherit from is visible only
when Profiles is not empty and Inherit label is true.
If the "inherit-mode" is active and user removes the selected profile (Label inherit from) from
profiles (Profiles). The Inherit label is set to unchecked. The value of Label is set to inherited value
from the profile.
See next slide for more fields.
Dialog
Similar to Label we define fields for description with equivalent functionality.
• Description - like Label.
• Inherit description - like Label.
• Description inherit from - like Label.
See next slide for more fields.
Dialog
• Domain - the user can select a class as a domain. The list should contains labels and support
search. The available classes must be from the current Model. When Model change the value
should go back to default. By default no value is selected.
• Range - the user can select a class as a range. The list should contains labels and support search.
The available classes must be from the current Model. When Model change the value should go
back to default. By default no value is selected.
• Range cardinality - user must select at least one cardinality option. The available options are "0-*",
"1-*", "1-1", and "*-*". The label and values a are equal in this case. By default the "0-*" is selected.
Data, submit and validation
The dialog should load data from URL specified by "data-source" URL input query argument.
There is as well as an example .
The dialog should contain "Create and clear" button. If the dialog state is valid and user clicks the
button, the JSON file according to should be send by POST (JSON in body) to URL
specified by "submit-url" URL input query argument.
If user try to click "Create and clear" the dialog should validate for the first time. Since that
validation should happen as user change any of the input fields. The dialog is invalid if:
• No model is selected. Message: "Model must be set."
• Label is in the "manual-mode" mode and is empty. Message: "Label must not be empty."
• Domain is not selected. Message: "Domain must be set."
• Range is not selected. Message: "Range must be set."
The validation message should be visible close to the validated input field. Text color should be
red.
JSON-schema input document
JSON-schema
Final remarks
• Do not forge to deploy your application!
Questions, ideas, or any other feedback?
Please feel free to use the anonymous feedback form.