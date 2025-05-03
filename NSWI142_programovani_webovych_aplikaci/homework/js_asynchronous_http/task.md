In this exercise we aim to implement a simple web form for filling a weekly report regarding allocation of work hours. The form is designed as a single-page application (SPA) and all data transfers are handled strictly with AJAX calls to given REST-like API.

Download a [starter pack](browser.zip), which already implements the user interface. Your objective is to implement a communication layer, which handles the data transfers (data model). The layer is represented by `DataModel` class in the `solution.js` file. Do not modify any other parts of the application and **submit only** the updates **`solution.js` file**.

### Data Model Interface

An object od `DataModel` class represent one data endpoint (which is represented by one URL). The URL of an endpoint (actually the URL prefix without the query part) is passed as the only parameter of the constructor. Beside the data communication, the `DataModel` also works as a transparent cache for the data.

Method `getData()` has to fetch all the data. It takes single argument -- the `callback` function which is invoked once the data are ready or when an error occurred. If the retrieval was successful, the `callback` is invoked with single argument holding the data. Otherwise it gets two arguments -- first (in place of the data) is `null` value, the second is a string with error message.

The data are returned as an array of objects, each object holding `id`, `caption`, and `hours` properties (unique identifier, textual representation of a hours category, and the actual amount of hours). However, the REST API was designed so that the categories and the hours for each category are returned separately. You will need to perform multiple requests and combine the results together.

Once a complete data retrieval is performed, the data are stored in internal cache. All subsequent calls of `getData()` must return the data from the cache by immediate (synchronous) invocation of the `callback`.

The `invalidate()` method cleans up the internal cache. The first subsequent call to `getData()` must perform the asynchronous HTTP queries to the REST API to retrieve the data again.

The `setHours()` method is passed an `id` of a record and a new `hours` value and updates it using asynchronous call to REST API. If the update was successful, internal records in cache are updated and `callback` is invoked without parameters. In case of failure, the `callback` is invoked with a single argument -- a string holding the error message.

When an asynchronous method (one with a callback) is invoked, you can rely on a fact that no other calls to that object will be performed until the `callback` is invoked. However, invoking data model methods inside a callback is perfectly legitimate.

### REST API

The REST API is represented by one URL, whilst calling different functions of the API is distinguished by the query parameters. The API always returns a JSON collection (object), which always holds an `ok` property with boolean value. This property indicates whether the call was successful or not. If error occurred, its message may be stored in `error` property. In case of success, the result data are in the `payload` property.

The API supports three functions altogether:

-   `GET` request without query parameters: Returns all categories as an array of objects where each object has `id` and `caption`.
-   `GET` request with `action=hours` and `id=<id>` query parameters where `<id>` is the identifier of the requested category record (e.g., `?action=hours&id=42`). It returns an object holding `id`, `caption`, and `hours`.
-   `POST` request with `action=hours`, `id=<id>`, and `hours=<hours>` query parameters and empty body: Sets the hours of category with given ID. This call returns no `payload` in the JSON response (only `ok` and possibly `error` in case of failure).

**Notice:** The error messages (strings) returned from REST API in the `error` property must be passed to the `callback` function of the data model interface without any changes, so we can test them properly in ReCodEx.

### Testing and Debugging

Your solution will be tested in ReCodEx using a [CLI script](https://recodex.mff.cuni.cz/api/v1/uploaded-files/91231466-f746-11e8-b0fd-00505601122b/download), which implements a simple mock of [`fetch()`](https://developer.mozilla.org/en-US/docs/Web/API/Fetch_API) function and standard [`Promise`](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Promise) objects. Do not use old `XMLHTTPRequest` API nor any other AJAX APIs (not even jQuery, despite the fact it is present in the starter pack). The mock does not implement all features of `fetch()` (e.g., it does not support `AbortController`). Try to use it as straightforward as possible, do not concern yourselves with details that are not explicitly required by this specification.

You need a web server with PHP (7.x) support if you are to debug your solution in a browser (using the starter pack). Perhaps the easiest way is to use [webik](https://webik.ms.mff.cuni.cz/) server and place youre code there. Do not forget to update the filesystem privileges so that the web server can modify `data.json` file in the `restapi` directory. As an alternative, you may try to set up a local webserver with PHP (e.g., using WAMP/LAMP packages). A third option is to debug only using the CLI script and [Node.js](https://nodejs.org/en/).