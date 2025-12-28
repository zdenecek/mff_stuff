

1. Model - the model we want to create the association into. 
1.1 The input property should allow user to select from available models. 
1.2 by default the first model is selected.

2. Specializations - 
2.1 the user can select none,
2.2 one,
2.3 or more associations to specialize. 
2.4 User can select values from all available associations.
2.5 By default no value is selected.

3. Profiles - 
3.1 the user can select none, 
3.2 one,
3.3 or more associations to profile. 
3.4 User can only select from associations that are in the currently selected Model. 
3.5 When user change Model all selected profiles must be removed.
3.6 By default no value is selected.


The dialog should allow user to inherit values from a selected profile. The idea is that the user can
either provide value manually (manual-mode) or inherit the value (inherit-mode). The "inheritmode"
is active when Inherit label is checked an a profile is selected in Label inherit from.
4. Label - a text input for a label.
4.1 When in the "manual-mode" the user can enter a value directly.
4.2 When in the "inherit-mode" the input is in a read only mode, showing the inherited value.
4.3 The "manual-mode" value must not be lost by switching to the "inherit-mode" and back.
4.4 Inherit label - a check box indicating whether label should be inherited or not. 
4.5 The Inherit label is visible only when Profiles is not empty.
4.6 Label inherit from - the user can select a profile from Profiles. 
4.7 The Label inherit from is visible only when Profiles is not empty and Inherit label is true.
4.8 If the "inherit-mode" is active and user removes the selected profile (Label inherit from) from
profiles (Profiles). The Inherit label is set to unchecked. The value of Label is set to inherited value
from the profile.

Similar to Label we define fields for description with equivalent functionality.
• Description - like Label.
• Inherit description - like Label.
• Description inherit from - like Label.


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