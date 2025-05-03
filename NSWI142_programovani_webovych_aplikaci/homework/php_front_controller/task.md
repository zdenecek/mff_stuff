Design and implement a _bootstrap_ script for a web application in PHP that employs [Front Controller](https://en.wikipedia.org/wiki/Front_controller) design pattern. The application has a single entry point -- `index.php` script which is responsible for generating all HTML pages.

The script is guided by a URL parameter `page`, which identifies the page that should be displayed. It refers to a target `.php` file with a template to be displayed. The template is automatically placed between common header and footer. The template files may require additional URL parameters. If they do, the bootstrap is responsible for validating them and passing them on to the template.

In detail, the `index.php` script should perform the following steps:

1.  Determine the target template file from the `page` parameter in URL. If the target template file does not exist, the script terminates returning `404 Not Found` HTTP response.
2.  Attempts to locate parameters descriptor -- list of URL parameters expected by the template. If the descriptor exists, the parameters are loaded and verified.
3.  Common header from `templates/_header.php` is loaded and passed to output using `include` or `require`.
4.  The template identified in the first step is loaded. Parameters from the second step are passed to the template and the template is yielded to the output.
5.  Common footer from `templates/_footer.php` is loaded and passed to output.

In case any URL parameters are missing or does not conform to expected format, the bootstrap script ends with HTTP code 400 (Bad Request). In case of any other error (more precisely, when included `.php` file throws an exception), the script should end with HTTP code 500 (Internal Server Error). In case of an error, the actual contents of the HTTP body is not important (ReCodEx does not test it); however, it is recommended to put the actual error message there for the purposes of debugging.

### Template Files

All template files are located in `templates` directory and its subdirectories. Parameter `page` contains a relative path inside `templates` directory. The path may point to a particular file or to a directory. The value of `page` may contain only letters of English alphabet and slashes and the whole string must be a valid relative path (meaning there cannot be two subsequent slashes and there cannot be a slash at the beginning nor the end of the string) due to security reasons. This limitation prevents accessing files outside `templates` directory (since `..` contains dots which are invalid characters). It also prevents including `_header.php` or `_footer.php` as the main contents (since underscore is invalid character).

If the parameter `page` is missing or does not conform to the prescribed format, the script should end with HTTP code 400 (Bad Request).

Template files must end with `.php` extension. However, this extension must not be present in `page` parameter, so it has to be added automatically. Furthermore, if the parameter `page` refers to a directory, the target template file is `index.php` in that directory. For instance, let us have

```
?page=foo/bar
```

and `templates/foo/bar` is an existing directory, then the target template file is `templates/foo/bar/index.php`. If no such directory exists, the template file is `templates/foo/bar.php`.

If the target template file (determined by the rules above) does not exist, the bootstrap scripts end with HTTP code 404 (Not Found).

### Validation of Template Parameters

The parameters descriptor is stored in a file, which has the same name and relative path as the template file, but it is located in `parameters` directory. I.e., a template file

```
templates/foo/bar.php
```

have its parameters descriptor stored in

```
parameters/foo/bar.php
```

If no such file exists, it is assumed that the template does not require any URL parameters.

The descriptor itself is an array, which is yielded from the descriptor file by `return` statement (which causes that the descriptor is returned from the `include`/`require` invocation). The keys in the array are names of the parameters, values restrict types of the parameters. Possible type restrictions are the following:

-   an array of strings lists allowed values (i.e., the parameter value must match one of the strings in the array)
-   `'int'` - the value must be a number in decimal format and it is passed to the template as integer
-   `'string'` - the value must be a string; since all URL parameter values are strings by default, it means that only the presence of the parameter is tested

The loaded (validated) parameters should be passed to the template as if they were their own variables. For instance, if the template has a descriptor

```
[ 'foo' => 'int' ]
```

it expects that variable `$foo` holding an integer will be present in its context.

However, it is not a good practice to pass on the parameters as global variables. A simple solution would be to create a function which loads the parameters as local variables and then performs `include`/`require` to load the template (i.e., the template will be evaluated in the context of the function). In order to test this restriction properly, we need to **forbid any usage of global variables** which have names that corresponds to the parameter names (actually, it is a good idea to refrain from using global variables completely).

To simplify the situation, the parameter names are guaranteed to comprise only the letters of English alphabet.

### ReCodEx Submission

You may use this [starter pack](debug.zip) for debugging. Submit only the **`index.php` file** into ReCodEx. If you need to divide your code among multiple `.php` files, you may submit them as well; however, the files must be in the same directory as `index.php` (ReCodEx does not support directories in file submission) and no file name may begin with `recodex` prefix. Furthermore, do not use any classes, functions, nor global variables which are prefixed with `recodex` in your code.

Please, be advised that your solution will be tested in a virtual environment (without actual web server). Do not use any functions of HTTP wrapper, which are not required by the specification. To set the HTTP response code, use [`http_response_code()`](http://php.net/manual/en/function.http-response-code.php) function.