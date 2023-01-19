Create a CSS layout for existing web page. The HTML file along with figures is already prepared for you in [a zip file](page.zip). **Submit only the `style.css` file into ReCodEx** (styles must work on original HTML document).

The main objective is that the layout will adapt to both small and large screens. We would define _narrow_ view as a situation when the web page is rendered in a window smaller than `1000px` in width. Otherwise we call it _wide_ view.

In the narrow view, the navigational panel is displayed at the top of the page (full width of the viewport and adequate height). The navigational panel is pinned to the window of the browser and the page scrolls underneath it. The page adapts its width to the width of the window, but it is never narrower than `400px`.

In the wide view, the navigational panel is displayed on the left, stretched to the full height of the window and with fixed width of `200px`. When the page scrolls, the panel remains on its placed pinned to the window. The rest of the page has fixed width of `800px`. When the window width grows beyond the `1000px`, the contents of the page is centered in the remaining place of the window without the navigational panel (e.g., if the window is `1200px` wide, the horizontal space is taken as follows: `200px` navigational panel, `100px` space, `800px` article contents, `100px` space).

Furthermore, your design needs to fulfill the following criteria:

-   Only the main page (entire document, except for the navigational panel) scrolls. Do not use `overflow: auto` nor `overflow: scroll` in your design.
-   In the narrow view, the hyperlinks in the navigation menu are not shown as list items of unordered list but rather as normal hyperlinks ordered from left to right and centered (with appropriate margins among them).
-   Maximal allowed width for the images is `400px`. In the wide view, the `.floatright` and `.floatleft` images are displayed as floating figures on the right and left side respectively, so the text flows around them. In the narrow view, all images are displayed as centered block elements.
-   Make sure that the footer of the page looks consistent with the article. It means it should be aligned in the same manner and scroll along with the article.

Please, focus mainly on fulfilling the outlined criteria in your implementation. You may add suitable margins or paddings to make the page look adequately. However, do not bother with special effect, colors, or backgrounds which are not related with the responsive design. It will simplify the evaluation process of your solutions.

Layout example (wide view):  
![Navigation on the left (wide view)](example-left.png "Navigation on the left (wide view)")

Layout example (narrow view):  
![Navigation on the top (narrow view)](example-top.png "Navigation on the top (narrow view)")