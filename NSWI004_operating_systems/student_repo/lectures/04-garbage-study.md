# Process Memory Layout

This is the last of three self study modules dedicated to process memory layout,
dealing with managed (garbage collected) heaps. The goal is to explain basic
concepts related to garbage collection (especially reachability) and again
give a reasonable impression of what overhead to expect when using heap
with a garbage collector.

At the end of this module, you should be able to:

- *explain* the role of a garbage collector,
- *distinguish* garbage from live objects on sample program fragments,
- *discuss* the differences in coding with garbage collector vs explicit heap management,
- *explain* how reference tracing or counting is used to distinguish garbage from live objects,
- *describe* the mark-and-sweep garbage collector algorithm,
- *describe* the copying garbage collector algorithm,
- *state* the generational hypothesis,
- *describe* a generational garbage collector,
- *describe* where the garbage collector algorithms above obtain input information, specifically
    - the collection roots,
    - the inter-object references,
    - the inter-generation references,
- *describe* the issues associated with concurrency between the collector and the mutator,
- *discuss* the overheads associated with garbage collection,
- *compare* the overheads with explicit heap management.

Typically, heap implementations with garbage collection reside in runtime environments
of high level programming languages (such as the Java Virtual Machine or the Common
Language Runtime). Strictly speaking, they are not a part of an operating system,
we therefore will not use our favorite book for this lecture, but instead
look at web references.


## Heap Object Lifetime

Each object on the heap has a useful lifetime, from the moment it was allocated
to the moment it was last used. By definition, at the end of its useful lifetime,
the object only takes up heap space, which should be reclaimed. In _traditional_ heap
implementations, this is done by explicitly calling `free` (or `delete` or other explicit
object deallocation operations in the target programming language). In _managed_ heap
implementations, the heap manager takes care of releasing the objects automatically.

The problem, of course, is how to efficiently and reliably recognize when an object
reaches the end of its useful lifetime. In general, this would require forecasting
future program behavior, which is not practically possible. The managed heap
implementations therefore introduce a small but important tweak: rather
than releasing objects that _will_ no longer be used, objects are
released when they _can_ no longer be used.

To illustrate the difference, consider the following Java snippet:

```java
public class AnExperiment {
    public static Object anObject;
    public static void aMethod () {
        Object anotherObject;
        anObject = new Object ();
        anotherObject = new Object ();
    };
}
```

When a caller invokes `AnExperiment.aMethod`, the program allocates two `Object`s,
with references stored in variables `anObject` and `anotherObject`, respectively.
Let us call these objects "first" and "second".

When the method returns, both "first" and "second" are on the heap, but while
"first" can still be accessed through the `anObject` variable, the program
no longer has any variable that references "second" (the `anotherObject`
variable was on the stack and ceases to exist when the method returns).

Without forecasting future program behavior, the managed heap implementation can
therefore release "second", but it cannot afford to release "first", even though
it may also well be at the end of its useful lifetime.


### Garbage Collection Terminology

In technical parlance, heap objects are called _live_ when usable by the program,
and _dead_ otherwise. Dead objects represent _garbage_ and the responsibility of
the heap implementation is to identify and _garbage collect_ dead objects.

The garbage collector must determine what objects can still be referenced
by the program. A program can obviously access any object whose reference
is stored in a live variable, these variables are called _roots_. In
addition to that, an object can be accessed by following references
from another object, such objects are transitively _reachable_.

Two essential approaches to determining reachability are _reference counting_
and _reference tracing_. With reference counting, the garbage collector
expects the runtime environment to maintain a counter of references
to each object. Obviously, when that count reaches zero for some
object, then that object is not reachable. With reference tracing,
the garbage collector traverses object references starting at the roots,
and marks all objects encountered as reachable, the rest is not reachable.


### Garbage Collection Algorithms

The useful knowledge we aim for is:

- Understanding basic ideas of reference counting. Reference counting is actually
  not used too often in local heaps (but it does have its place in distributed
  garbage collection), so read just to get the basic idea.

  As the source material, you can use:

    - https://www.memorymanagement.org/mmref/recycle.html#reference-counts

- Working of the basic mark-and-sweep algorithm. This is a very basic algorithm
  that captures the essence of reference traversal (in fact it is just trivial
  graph traversal of the kind you must have seen before).

  As the source material, you can use:

    - https://www.memorymanagement.org/mmref/recycle.html#mark-sweep-collection

- Extensions to the basic mark-and-sweep algorithm, in particular copying garbage
  collection and generational garbage collection. The former helps solve the problem
  of heap fragmentation, the latter reduces the work done in each garbage collection cycle.

  As the source material, you can use:

    - https://www.memorymanagement.org/mmref/recycle.html#copying-collection
    - https://www.oracle.com/webfolder/technetwork/tutorials/obe/java/gc01/index.html
    - https://docs.microsoft.com/en-us/dotnet/standard/garbage-collection/fundamentals

Obviously, there are many other sources, a lot of links can be found for example
at https://wiki.c2.com/?GarbageCollection. Do not go too deep, there are actually
[whole books](http://gchandbook.org) on garbage collection :-) ...


In the following, `System.gc` denotes a method that explicitly triggers garbage collection.
Also, please ignore the possiblity that the compiler optimizes the entire example away
(because the code has no externally visible actions), and assume it executes as
written.


**[Q1]** Look at the following code.

```java
class Data {
    public Data neighbor;
}

public class Main {
    public static void main (String [] arguments) {
        Data a = new Data (); // Data object "first"
        Data b = new Data (); // Data object "second"
        a.neighbor = b;
        b = null;
        System.gc ();
    }
}
```

At the moment `System.gc` is called, which of the two allocated `Data` objects can be released ?

<details><summary>Hint ...</summary>

> References, both direct and transitive from other live objects, keep an object live.

</details>


**[Q2]** Look at the following code.

```java
class Data {
    public Data neighbor;
}

public class Main {
    public static void main (String [] arguments) {
        Data a = new Data (); // Data object "first"
        Data b = new Data (); // Data object "second"
        a.neighbor = b;
        b.neighbor = a;
        a = null;
        b = null;
        System.gc ();
    }
}
```

<details><summary>Hint ...</summary>

> References from dead objects do not make an object live.

</details>


**[Q3]** For the purpose of this question, consider a hypothetical situation where
you could convert between references and integers in Java as you can in C (the
integer would contain the memory address that the reference points to).
Look at the following code fragment:

```java
public class Main {
    public static void main (String [] arguments) {
        Object o = new Object ();
        int a = reference_to_integer (o) + 1234;
        o = null;
        System.gc ();
        o = integer_to_reference (a - 1234);
    }
}
```

What would the ability to convert between references and integers (addresses) mean for garbage collection ?


**[Q4]** What is a good time for the garbage collection to happen ?


**[Q5]** Why are some garbage collectors called "stop the world" ?


## Eager For More ?

Want to delve into the topic beyond the standard module content ?
We've got you covered !

- About garbage collector implementations
    - A collector for C at https://www.hboehm.info/gc
    - G1 collector https://doi.org/10.1145/1029873.1029879
    - Azul C4 collector https://doi.org/10.1145/1993478.1993491
    - Shennandoah collector https://doi.org/10.1145/2972206.2972210
- About garbage collection overheads
    - Overheads in general https://doi.org/10.1145/1012888.1005693
    - Reference counting overheads in https://doi.org/10.1145/2258996.2259008
    - Comparison with explicit heap management in https://doi.org/10.1145/1103845.1094836
    - Parameter tuning in https://docs.oracle.com/javase/8/docs/technotes/guides/vm/gctuning/ergonomics.html
- About garbage collection uses
    - In the kernel with https://www.usenix.org/conference/osdi18/presentation/cutler
- More ways to manage memory ?
    - For example regions in https://www.aicas.com/cms/en/rtsj
