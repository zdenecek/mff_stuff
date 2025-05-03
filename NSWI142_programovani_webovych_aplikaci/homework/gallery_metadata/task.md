Consider the following scenario. You are preparing a web page displaying a photo gallery. The metadata of the images are already loaded in a JavaScript array. Your task is to pre-process the data, so the images are displayed in correct order and grouped together.

Implement a `preprocessGalleryData(imgData)` function which performs the preprocessing of image records in `imgData` and yields the results. The `imgData` argument holds input array of objects with the following properties (one object represents one image):

-   `url` -- unique URL of the image
-   `title` -- caption/label
-   `created` -- `Date` object holding the time of creation
-   `similar` -- array of similar images in the gallery (directly holding the objects of the `imgData` array)

The function returns the image objects organized in blocks (groups), so that each block is also an array (i.e., the function yields an array of arrays). Each block holds image objects ordered from the oldest one to the newest one. The blocks are also ordered from the oldest to the newest whilst each block is represented by its oldest image. The age of an image is primarily determined from the `created` property. If two images were created at the same time, we consider the image at the lower index of `imgData` array to be older than image at higher index. The best way to compare times is using `getTime()` method of `Date` object.

The aggregation into blocks is conducted by the following rules. Images, which are considered _similar_ (according to relations defined in the `similar` arrays) must be in one block. The similarity relation is equivalence (i.e., it is reflexive, symmetric, and transitive); however, not all relations has to be explicitly expressed in the `similar` fields. For instance, if image `A` is similar to image `B`, it is sufficient that `B` is on the `similar` list of `A` but `A` is not on the `similar` list of `B` (or vice versa).

Images which are not similar to any other images are considered _independent_ and they are grouped to blocks as follows. Let us take two subsequent blocks formed by _similar_ images whilst `A` is the oldest image in the first block and `B` is the oldest image in second block. Then, all independent images which are newer than `A` but older than `B` are grouped together in one block. If any such block remains empty, it will not appear in the output.

Submit only **solution.js** in the ReCodEx. The file has to follow export rules of Node.js. Testing script with a skelet of `solution.js` file and sample testing data can be downloaded [here](https://recodex.mff.cuni.cz/api/v1/uploaded-files/b89a00e8-e619-11e8-a4be-00505601122b/download).