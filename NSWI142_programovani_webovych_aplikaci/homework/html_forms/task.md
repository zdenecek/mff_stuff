Create a HTML document that will contain a simple form simulating submission of a discussion post. Submit your solution as a single `*.html` file.

-   The document must contain a single form that submits data to `http://demo.stools.net` using the `POST` method.
-   The form must contain the following control elements (their values must be submitted to the server under the names listed below):
    -   Text input field named `username` for entering a user name.
    -   Text input field named `email` for entering an email address.
    -   Text input field named `age` for entering a numeric age in years.
    -   Multiline text field named `comment` for entering the actual comment text.
    -   Drop-down menu named `publish`, where the user can choose from options `public` and `private`.
    -   A functioning submit button. Make sure other ways for submitting the form supported natively by the browser work, e.g., pressing the `enter` key (important for [accessibility](https://developer.mozilla.org/en-US/docs/Learn/Accessibility)).
-   Add the following validation rules to the elements in the form using only [HTML5](https://diveinto.html5doctor.com/forms.html).
    -   The input field for username and comment text are required (must be non-empty).
    -   The input field for email must contain a valid email address.
    -   Age must be a non-negative number.
-   In the `publish` drop-downu menu, the `private` value must be selected by default.
-   The submitted HTML document must contain a **valid** HTML code. ReCodEx validator is slightly more restrictive than W3C validator. It also reports suspicious constructions which might be formally valid, but which are considered to be a bad practice (e.g., omitting quotes around attribute values or using empty `<p>` element as a vertical space).

Note: Please bear in mind that ReCodEx is just a first-instance judge of correctness. Thus, having your solution pass the tests completely is a necessary first step; however, the final judgement is passed by the teacher. Furthermore, please note that the HTML validation and correctness checking is still experimental in ReCodEx. If you are reasonably sure your solution is correct even though it does not pass all tests, contact your teacher.