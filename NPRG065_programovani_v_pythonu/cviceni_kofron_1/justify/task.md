The task is to create a program that aligns the text to the block of the specified width (number of characters), i.e., each line has exactly the entered number of characters.

The text is read from the standard input and is printed out to the standard output.

The program expects one parameter on the command line, which is the specified line width (number of characters per line). If this parameter is missing or not a number or less than 1, the program prints the string `Error` and exits.

The words are separated by whitespace. To detect a whitespace, you can use the `isspace` method on instances of the string class. You can also assume that the `split` and `strip` methods on instances of the string class interpret whitespaces correctly.

An empty line (or more empty lines, whitespace-only line) forms a paragraph break. At the output, the paragraphs are separated by just one empty line. There is no empty line before the first and after the last paragraph.

The lines within a paragraph have the same output (specified) length. There is always at least one space between the words. If the given line would be shorter than the specified length, spaces are evenly added between the words. If the gaps cannot be evenly distributed, more spaces are added one by one between the words from the left. The last line of the paragraph is aligned to the left, i.e., there is always exactly one space between words and the line has a maximum specified length. If the word in the text is longer than the specified line length, then this whole word is on a separate line and exceeds the maximum entered line length. If there is only one word on a line, then it is aligned to the left.

The entire input may not fit into the memory. One paragraph always fits into the memory

Attached file [INPUT](https://recodex.mff.cuni.cz/api/v1/uploaded-files/1c7b302d-6796-11e9-b0fd-00505601122b/download) contains an example and file [OUTPUT](https://recodex.mff.cuni.cz/api/v1/uploaded-files/1c7b2a50-6796-11e9-b0fd-00505601122b/download) contains the correct solution for this example, when the specified length is 70. If you run your solution using the file INPUT (on a Unix system, for example, `your_file.py 70 <INPUT`), it must print out exactly what is in the OUTPUT file (if it does not, then do not submit your solution).