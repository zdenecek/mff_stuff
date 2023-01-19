The objective of this exercise is to validate and preprocess form data before they are sent to the server. Your job is to implement function `processFormData(formData, errors)` which takes form data on the input accompanied with empty object `errors` that records all errors found. The result of the function is a serialized JSON string with transformed form data.

The form being submitted is implemented in the prepared [HTML file](browser.zip), which can be useful for debugging. The form represents a fictive web application for scheduling ReCodEx hacking operations. The user sets the parameters of the HTTP requests (with possible repetitions) and schedules their execution.

The form has a single element named `url_base` where the user fills the base URL (prefix) of ReCodEx API. It is followed by a sequence of operations (HTTP requests) where each operation holds the following properties:

-   `date` - date when the request is scheduled,
-   `time` - time when the request is scheduled (it may also hold a time interval, if the request is repeated more than once),
-   `repeat` - how many times should the request be repeated,
-   `url` - relative URL of the request (final URL is a concatenation of `url_base` and this relative URL),
-   `method` - HTTP method of the request,
-   `body` - body of the request.

Even if the form holds more than one scheduled operation, all operations share the names of the inputs above. This might be impractical should the form be processed directly by a PHP script. However, preprocessing the data using `FormData` methods will in fact make the job easier.

### Validation Rules

The properties of each operation must fulfill the following requirements:

-   The date must be in `d.m.yyyy`, `m/d/yyyy`, or `yyyy-mm-dd` format. The `yyyy` represents four-digit year, `d` and `m` represent day and month respectively. `dd` and `mm` represent also day and month but padded by zero from the left to ensure two-digits for each value.
-   The time must be in `h:mm` or `h:mm:ss`, where `h` stands for hours (24-hour format), `mm` are minutes, and `ss` are seconds. Minutes and seconds are padded from the left so they always take two digits. A time interval has format `time-time`, whilst between the times and the dash may be arbitrary number of spaces.
-   The repeat count is a number from 1 to 100 (as suggested in the HTML testing page). If the repeat count is greater than 1, the time field may contain a time interval instead of a single time. If this rule is violated, the error should be reported at the `time` field.
-   The method must be `GET`, `POST`, `PUT`, or `DELETE`.
-   The body is either empty string, or it must hold a valid JSON string.

The URL validation is slightly more complex. Therefore, we have already implemented (simplified) validation functions for base URL (i.e., the `url_base` property) and relative URLs (i.e., the `url` properties) in the prepared **solution.js** file. Use them.

### Data Preprocessing

Besides the validation, the form data has to be transformed into JSON. The result is structured as a list of collections (i.e., an array of objects), where each collection represents one operation from the form. The order of collections in the output must match the order of operations in the form. The names of the collection properties match the names of form input elements; however, some values has to be transformed into different formats:

-   `date` has to be returned as unix timestamp (represented as number) of the midnight of given date in UTC. Beware that the JavaScript `Date` class creates time records in the local time zone based on system settings by default.
-   `time` is represented as the number of seconds from the midnight (i.e., again as a number). Time interval is represented as an object with items `from` and `to` where both items are time values.
-   `repeat` is simply converted to a number.
-   `url` is concatenated with the `url_base` prefix.
-   `body` is parsed from JSON string and returned as the corresponding structure (object or array). Empty body is represented as empty object `{}`.

### Errors

In case there are errors detected in the validation, the `processFormData(formData, errors)` function returns `null` instead of JSON string. All errors are reported by setting properties of the `errors` object. If `url_base` validation fails, the error message is assigned to `errors.url_base` property.

Validation failures of operation properties are reported as follows. Each failed property has an object assigned to `errors.<name>` (where `<name>` is the name of the property) and each error is reported under a key that corresponds to the index of the operation (counted from 0). For example, if the second and the fifth operation (counted naturally from 1) have invalid dates, the result `errors` object will look like this:

```
{
  date: {
    1: 'Invalid format.',
    4: 'Invalid format.'
  }
}
```

The actual error messages may be arbitrary. The tests only validate that they are nonempty strings.

### Testing and Submission

Testing framework and a sample of testing data may be downloaded [here](cli.zip). Please note, that we use our mock implementation of `FormData` class so it may not behave exactly as its counterpart in the browser. Therefore, we strongly recommend to follow the specification of the form data interface rather than to rely on any behavioral details you might have observed during debugging in the browser.

Only the **solution.js** file should be submitted in the ReCodEx. Its implementation has to follow the Node.js module export protocol (as outlined in the testing framework).