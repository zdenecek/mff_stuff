# Process Memory Layout

This is a graded quiz, do not forget to commit and push your responses in time (deadline is end of October 26).


## Finalizers

**[Q1]** Python represents open files by objects with methods such as `read` and `write`.
The interface of these objects is described in the [documentation](https://docs.python.org/3/library/io.html).
When browsing the documentation, you can see that the file objects actually also have a `close` method,
but when looking into Python sources, you see that the file objects are closed when garbage collected
(the code is somewhat simplified):

```python
class IOBase (metaclass = abc.ABCMeta):
    def __del__ (self):
        try:
            closed = self.closed
        except AttributeError:
            return

        if closed:
            return

        try:
            self.close ()
        except:
            pass
```

Given that Python uses garbage collection, and that file objects are closed when garbage collected,
can you explain why the `close` method is still included in the interface ?

**[A1]** ... **[/A1]**


## Reference Tracing

**[Q2]** This question expects you to look up some data and make educated guesses.
Where the question does not provide enough information, substitute reasonable
values and include such values in your answer.

Assume a managed heap with a garbage collector. The heap is 128 GB large and currently
filled with objects that all have around 10 small attributes, such as integers,
and around 3 references to other objects.

Estimate how long it may take a mark and sweep garbage collector to perform the mark phase,
if memory access speed is the main limiting factor, and around 30% of the objects on the
heap are live. How will that answer change if around 80% of the objects are live ?

**[A2]** ... **[/A2]**


## Generational Collection

**[Q3]** Assume an imaginary programming language, with syntax inspired by C# and Java,
but without the pesky visibility attributes. Objects are constructed with `new` and
passed around using references. Look at the following code fragment:

```java
class Something { ... }

class Main {
    static void main () {
        Something a = new Something ()
        Something b = new Something ()
        // Your code here ...
    }
}
```

If the language were to use a generational garbage collector,
the object instances referenced by `a` and `b` would likely
be allocated in the young generation. What code would you
have to insert in place of the comment to get the garbage
collector to promote both objects to the old (tenured)
generation ?

**[A3]** ... **[/A3]**


## Ask Your Question

**[Q0]** Ask a single question concerning the topic of the next self study module.

**[A0]** ... **[/A0]**
