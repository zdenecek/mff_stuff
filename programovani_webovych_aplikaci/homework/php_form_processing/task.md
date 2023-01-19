The objective of this exercise is to correctly process a HTML form by a PHP script and save its values. If some parts of the form are not valid, an error should be reported. The form itself is already prepared in the [starter pack](starter_pack.zip), in `form_template.html` file as a complete HTML document.

Your task is to write the contents of the bootstrap script `index.php`. The script should process `GET` requests by returning the HTML page (the prepared template). The `POST` requests are expected to be form submissions and should be processed accordingly. It means, the data from the form has to be validated and, if correct, they should be saved using given function. If the validation fails, all errors should be reported (also using given function).

After the form is processed, the script should perform **redirect** -- i.e., HTTP response code 302 or 303 is returned and appropriate header hold the (relative) URL pointing to the bootstrap script (self redirect).

To simplify your work as well as subsequent testing, the following functions are already prepared for you in `recodex_lib.php` file:

-   `recodex_save_survey()` -- saves the data from the form in case the validation was successful
-   `recodex_survey_error()` -- reports the errors in case the validation failed

Both function have trivial implementation, which should help you with debugging. For instance, the first one saves the data into a file in JSON format, so you can verify the form has been indeed processed. The testing suite provides different implementation of these functions.

_Notice:_ Function `recodex_survey_error()` gets an error message as a parameter. Testing particular error messages would be impractical; hence, only the second argument (the list of invalid fields) is tested automatically.

### Validation Details

The form is a Christmas questionnaire. It holds the following fields:

-   `firstName` -- nonempty string (mandatory)
-   `lastName` -- nonempty string (mandatory)
-   `email` -- valid email address (mandatory)
-   `deliveryBoy` -- who delivers the presents; actual values can be found in the source code
-   `unboxDay` -- day when the presents are unboxed (number `24` or `25`)
-   `fromTime`, `toTime` - interval of opening the presents, 24h format `h:mm` (both values are optional)
-   `gifts` -- wishlist, possible values can be found in source code
-   If value `other` is on the `gifts` list, the `giftCustom` field is expected to hold a nonempty string

The validation process checks that the fields are all present in the sent data and their values have correct format. You may find the details in the HTML form itself. Focus on validation attributes wich will provide necessary information. In particular, the text inputs have `maxlength` and `pattern` attributes, other inputs (`radio`, `select`, `checkbox`) hold the possible values in the `value` attribute.

Let us notice that different form inputs exhibit different behavior. For instance, if the user does not fill in `fromTime` or `toTime`, these fields will still be present in the form data and their values should be empty strings. If these fields are missing completely, something went wrong with the form submission. On the other hand, the `checkbox` input may not appear in the dataset at all, if the field is not checked. It means that the `gifts` array may not even exist even if the form is submitted correctly.

Furthermore, some items should be converted into appropriate format for saving:

-   The `unboxDay` value must be passed on as an integer.
-   The `fromTime` and `toTime` values must be converted into an integer representing number of minutes from the midnight. If the value is not filled in, it should be converted to `null`.
-   The `giftCustom` value is accessed only if `gifts` list holds `other` value. If `other` is missing, the `giftCustom` value for saving must be `null` (regardless of what was in the form data). On the other hand, if `other` is present in `gifts`, the `giftCustom` must hold a nonempty string.

### ReCodEx Submission

Submit only the **`index.php` file** into ReCodEx. If you need to divide your code among multiple `.php` files, you may submit them as well; however, the files must be in the same directory as `index.php` (ReCodEx does not support directories in file submission) and no file name may begin with `recodex` prefix. Furthermore, do not use any classes, functions, nor global variables which are prefixed with `recodex` in your code.