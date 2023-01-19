Your objective is to implement routing and dispatching mechanism to REST API framework. The REST API may be perceived as a sophisticated form of [RPC](https://en.wikipedia.org/wiki/Remote_procedure_call) over HTTP. HTTP request is transformed to a method invocation on a target class (controller). The returned value of the method is serialized to JSON and yielded as HTTP response.

The [starter pack](starter_pack.zip) already holds a `router.php` file with `Router` class. It has empty `dispatch` method which needs to be implemented. The start pack also holds a testing HTML frontend `test.html`, which performs calls to our REST API using AJAX.

The routing and dispatching is directed by URL parameter `action`. It defines both the target controller class and its method. The parameter is a sequence of two or more substrings separated by a slash. All substrings except the last one constitute a path (inside `controllers` directory) to the target `.php` file where the controller resides. The last substring selects the method of the controller. The path omitis the `.php` extension of the file, so it needs to be appended. Furthermore, the name of the controller class is determined from the name of its file by adding `Controller` suffix. Finally, the name of the target method is composed from the name of HTTP method used (in lower case) and the last substring from the `action` parameter.

I.e., if a `GET` request is performed holding `action=Foo/Bar/Spam/Eggs` in URL parameters, the `dispatch()` method should:

1.  Load `controllers/Foo/Bar/Spam.php` file.
2.  Create a new instance of `SpamController` class.
3.  Invoke `getEggs()` method on that instance.
4.  Serialize the returned value into JSON any yield it to the output.

To streamline the situation, let us restrict the substrings in `action` parameter so that they are allowed to comprise only letters and underscore.

### Handling Specific Situations

Borderline situations and errors are handled by setting appropriate HTTP response code. Optionally, the body may hold an error message. The ReCodEx tests only the HTTP response codes, so the actual text of such message may be arbitrary.

-   If the `action` parameter is missing or is not valid (contains invalid characters, does not have at least two substrings, ...), the dispatcher should yield `400` (`Bad Request`) HTTP response code.
-   If the target file does not exist, or the file does exist but it does not contain the appropriate controller class, or the class exists but it does not have the target method, the dispatcher should set the `404` (`Not Found`) code.
-   If an exception is thrown from the target method, the dispatcher should set `500` (`Internal Server Error`) response code and write the exception message to the output.
-   If everything goes smoothly, but the target method returns `null` (so we have nothing to serialize into JSON), the dispatcher should yield `204` (`No Content`) response code.

### Extension - Method Arguments

Optionally, you may implement the following extension. The target method may have arguments. The dispatcher should analyse the method using reflection and fill in the arguments from the HTTP parameters. If HTTP method GET is used, the parameters are stored in URL. If POST method is used, the parameters are in the body of the request. The names of the parameters match the names of the arguments of the target method.

For instance, if the method is declared as `getData($foo, $bar)`, the arguments should be taken from URL parameters `foo` and `bar` (accessible in the `$_GET` array or via `filter_input` function).

If some of the arguments are missing in the parameters, the dispatcher will yield `400` (`Bad Request`) HTTP response instead of invoking the target method.

You will need the following parts of reflection API to solve this extension. Class [`ReflectionMethod`](http://php.net/manual/en/class.reflectionmethod.php) (`getParameters()` method) and class [`ReflectionParameter`](http://php.net/manual/en/class.reflectionparameter.php) (`getName()` method).

### Submitting in ReCodEx

Please, **submit only `router.php`** file into ReCodEx. Do not create any other classes, functions, nor global variables in this file. On the other hand, you may modify the `Router` class to your liking, you need only to preserve the interface of `dispatch()` method.

A correct implementation of the basic version will be awarded by 50% of points. The other 50% can be received for implementing the method arguments extension.