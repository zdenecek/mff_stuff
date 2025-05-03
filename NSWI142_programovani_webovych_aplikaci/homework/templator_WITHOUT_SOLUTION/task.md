This exercise aims to demonstrate the templating fundamentals. Your task is to write a compiler that transpills given HTML template (in format specified below) into PHP-interleaved format (i.e., a regular PHP file that can be used as a template). The main objectives of this system are to make the template code more readable, to ensure the control structures are correctly enclosed, and to sanitize all data inserted into the output. However, we do not want to give up advantages of PHP-interleaving (efficient processing, using expressions in outputs, ...).

### Template Format

Template is a text file (most often HTML), which may comprise the following markers:

-   `{= expr}` - inserts the result of the expression safely into HTML (i.e., it is equivalent with PHP construct `<?= htmlspecialchars(expr) ?>`)
-   `{if cond}` and `{/if}` - the contents between these two markers is included in the output only if condition `cond` is met (equivalent of `<?php if (cond) { ?>` and `<?php } ?>`)
-   `{for expr}` and `{/for}` - the contents between these two markers is iterated based on the for-loop expression (equivalent of `<?php for (expr) { ?>` and `<?php } ?>`)
-   `{foreach expr}` and `{/foreach}` - the contents between these two markers is iterated based on the foreach expression (equivalent of `<?php foreach (expr) { ?>` and `<?php } ?>`)

Expressions inside the markers (designated by symbols `expr` and `cond`) are not interpreted by the templating system. They are merely copied into their PHP counterpart structures. The only conditions are that the expressions do not contain `{` nor `}` characters and they are not empty. Paired markers (`if`, `for`, and `foreach`) may be nested, but correct pairing must be ensured.

Characters `{` and `}` encountered outside markers are simply copied into the output. For instance, text `{four}` is copied without any interpretation since it is not a marker. On the other hand, text `{for }` is a valid marker (only with empty expression), so the system will raise an error during compilation.

### Interface

Please download the [starter pack](https://recodex.mff.cuni.cz/api/v1/uploaded-files/5d87a379-27a1-11eb-8e81-005056ad4f31/download) first. It contains (besides of an example template and its utilization) a `templator.php` file with `Templator` class skeleton declaration. Please make sure you keep the interface of the outlined public methods. This file is also **the only file** you should submit to ReCodEx. Method `loadTemplate()` loads data of given template file, but does not process them. Method `compileAndSave()` performs the actual compilation (transpill) and saves the result into given file. Calling `compileAndSave()` before a template is loaded is an error. All errors must be reported via exceptions. Every exception must be instance of `Exception` class (or derived class) and the actual error messages are not tested by ReCodEx.

You may declare other classes or functions in your solution, but do not write any code that would be directly executed during inclusion of `templator.php` file. Also do not modify any global variables and do not write anything into the output (not even error messages). You may use `compile.php` CLI PHP script to debug your implementation (similar script is used for testing in ReCodEx).