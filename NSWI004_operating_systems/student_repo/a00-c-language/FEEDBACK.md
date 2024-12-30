# Feedback for a00-c-language

This file contains feedback for assignment A00. We decided to use a format of
commenting on various code fragments that we find somehow interesting. We
mostly comment on code that we believe can be improved.

In programming, _the right_ approach to a particular problem very much depends
on the _context_ in which the problem appears. Developers therefore always need
to be aware of -- and balance -- different forces and concerns.
In addition to ensuring functional correctness, these include design choices
that influence code complexity, clarity, and maintainability, as well as
performance. Proper code layout is just a necessary condition.

Consequently, our comments are not meant to be interpreted as strict directives
on how to do things, but rather as alternatives that are worth considering. We
try to point out the key concerns in the examples, but in some cases our
approach might be just a matter of personal opinion or experience.

Feel free to discuss the comments during labs or open an Issue on the Forum.
Discussion is welcome and we will gladly provide further feedback if needed.


## Strive for consistency

This is one of the basic rules to make your code maintainable.
You will soon work in teams of three and if every member of the team used
his/her own style (code indentation, identifier naming), the code would soon
become unmanageable. Or rather, each team member would end up with a piece of
code the others would be afraid to touch because of a relatively high initial
effort needed for _reading/understanding_ the code.

To simplify this for you, we provide a C style definition based on Clang that
can check and even format your code automatically. Feel free to update it to
your liking (we provided what we think are sane defaults) but please be
consistent across your entire code base.

For example, encountering functions such as `convertLongToHexString` or a
boolean `IsFound` in the middle of otherwise `snake_case` code certainly does
not  improve readability. Quite the opposite, because the inconsistency keeps
sapping the reader's attention. Ignoring indentation, lack of white space
around elements, or mixing tabs with spaces falls into the same category.


## Avoid complex statements

Try to avoid constructs that might be difficult to read. You may be a Linux
kernel hacker but perhaps your team mates are not. They are perhaps not only
learning about kernel programming but also about the C language itself.

For example, we believe that the following constructs are best avoided, because
they complicate reasoning about the code. Note that saving one line of code
does not necessarily save time needed by a human to read _and_ comprehend the
code, and the compiler will optimize things as it sees fit anyway.

```c
buf[ptr--] = '0' + (10 - num % 10) % 10;
```

The right hand part of the assignment requires quite a bit of thinking, so
it should not be complicated further by reasoning about the priority of the
`--` operator.

Similarly, the following code can be easily split into two lines, removing
any doubt about the ordering of the decrement and pointer dereference.

```c
putchar(*--current);
```

## Pay attention to names

Names are very important. Programming is very much about creating a vocabulary
of concepts that you can use to "talk" about solution to a problem. Use 
variables to give names to values, use functions to give names to operations
and logical steps they consist of, use constants to give names to specific
values.

As a rule of thumb, identifiers with smaller scope (visibility, lifespan) can
be shorter, because the surrounding context provides additional information.
In contrast, identifiers with large scope or long lifespan should be more
descriptive, to make their meaning more explicit.

That is why it is fine to have `i` as loop variable (the loop body is usually
relatively small), but you do not have `pr` as the name for printing function.
Note that the C standard library is not the best example -- the short
names are a legacy of times when the length of identifiers was strictly
limited. Instead, consider the `pthread` library as a good example among
C libraries.

As an example, the following code is rather unreadable, especially if `pointer`
is usually abbreviated to at least `ptr`. Also note that because the function
accepts `long`, it completely hides the _intent_ of working with pointers.

```c
void pr_po(long i) { ... }
```

In this case, we want to indicate that the function should work with numbers
that represent addresses, i.e., the numerical value of a pointer. This would
be best indicated by using the `uintptr_t` type in addition to a better name,
such as `address` instead of `i`.

Finally, mind the spelling. Misspelling may shift meaning (e.g., "standard"
vs "standart", which means a flag in Czech) and, similarly to inconsistent
formatting, keeps nagging the reader.


## Strive for simplicity

There is a saying that _good programmers are lazy programmers_. We should
also add that it only holds _for some definitions of lazy_, so here we put
the old adage into a bit of context.

In general, you should actively try to reduce the complexity of your code
by using symbolic names for values as much as possible, avoiding code
duplication, and splitting code into functions corresponding to logical
steps.

### Make use of constant and C arrays

```c
char dec2hexchar(int digit, bool alpha_capitals) {
        if (digit < 10) {
                return digit + 48;
        }
        if (digit < 16) {
                return digit + (alpha_capitals ? 55 : 87);
        }
        return 0;
}
```

Do not expect everyone to remember the ASCII table by heart. Some people _may_
remember that the ASCII code of the letter `A` is 65 (or 97 in case of `a`),
so at least inserting the code with these numbers (and subtracting 10) would
make it _a bit_ better, but that's besides the point.

The point is that we can easily leverage the fact that `char` is implicitly
converted to `int` and replace the code above with `digit + '0'`. Note that
this also means that a loop for finding the end of a string can simply check
for zero, i.e., `while (*it != 0)` instead of `(*it != '\0')`.

It is also possible to use array indexing as an alternative:

```c
const char DIGITS_UPPER[] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    'A', 'B', 'C', 'D', 'E', 'F'
};
const char* DIGITS_LOWER = "0123456789abcdef";
```

Both forms are virtually the same and equally _obvious_. Even though
`DIGITS_LOWER` contains one extra byte (terminating zero), it is
definitely easier to type. With these constants, the code would be
simplified to the following:

```c
char dec2hexchar(int digit, bool alpha_capitals) {
    assert((digit >= 0) && (digit < 16));
    if (alpha_capitals) {
        return DIGITS_UPPER[digit];
    } else {
        return DIGITS_LOWER[digit];
    }
}
```

We have also seen several instances of the following code, which is
extremely error-prone and difficult to maintain:

```c
switch (value) {
    case 10: ;
            putchar('a');
            break;
    case 11: ;
            putchar('b');
            break;
    case 12: ;
            putchar('c');
            break;
    case 13: ;
            putchar('d');
            break;
    case 14: ;
            putchar('e');
            break;
    case 15: ;
            putchar('f');
            break;
}
```


### Avoid unnecessary computations

We have seen the following fragment in various instances.

```c
void print_long_long(long long n) {
    int position = count_digits(n, 10);
    char buffer[50];
    while (n > 0) {
       buffer[position] = ...;
       ...
    }
}
```

Note that counting the digits is not really needed, which turns it into extra
code that needs to be debugged. Using a bigger-than-necessary buffer is fine
(even though 50 is almost twice as much, but the stack can take it), but then
it should be equally fine to start filling the buffer from the end, or fill it
from the beginning then print the contents in reverse order.


### Parametrize code to avoid code duplication

The following was present in many sizes and flavors:

```c
static void prHex(unsigned int i, int Capital) {
    if (i == 0) {
        putchar('0');
        return;
    }
    unsigned int divisor = 1;
    while (i / divisor > 15) {
        divisor *= 16;
    }
    while (divisor != 0) {
        if ((i / divisor) % 16 > 9) {
            putchar((Capital ? 'A' : 'a') + (i / divisor) % 16 - 10);
        } else {
            putchar('0' + (i / divisor) % 16);
        }
        divisor /= 16;
    }
}

static void prPtr(uintptr_t i) {
    putchar('0');
    putchar('x');
    if (i == 0) {
        putchar('0');
        return;
    }
    uintptr_t divisor = 1;
    while (i / divisor > 15) {
        divisor *= 16;
    }
    while (divisor != 0) {
        if ((i / divisor) % 16 > 9) {
            putchar('a' + (i / divisor) % 16 - 10);
        } else {
            putchar('0' + (i / divisor) % 16);
        }
        divisor /= 16;
    }
}

static void pruInt(unsigned int i) {
    int divisor = 1;
    if (i == 0) {
        putchar('0');
        return;
    }
    while (i / divisor > 9) {
        divisor *= 10;
    }
    while (divisor != 0) {
        putchar('0' + (i / divisor) % 10);
        divisor /= 10;
    }
}
```

Note how the code repeats itself over and over again. The first iteration
that should be done by everybody is to replace the `prPtr` (note again the
inconsistency between the name and the rest of the code) with the following:

```c
static void prPtr(uintptr_t i) {
    putchar('0');
    putchar('x');
    prHex(i);
}
```

Actually, there should be also check for `NULL`:

```c
static void prPtr(uintptr_t ptr) {
    if (ptr == 0) {
        prStr("(nil)");
    } else {
        prStr("0x");
        prHex(ptr);
    }
}
```

More importantly though, code can be squeezed into one general function
that can be called from modifier-specific functions, all properly named
of course.

```c
static void print_unsigned(unsigned long long value, int base, const char* digits) {
    if (value == 0) {
        putchar('0');
        return;
    }
    unsigned long long divisor = 1;
    while (value / divisor > base - 1) {
        divisor *= base;
    }
    while (divisor != 0) {
        putchar(digits[(value / divisor) % base]);
        divisor /= base;
    }
}

static void print_hex(unsigned int i, bool capital) {
    if (capital) {
        print_unsigned(i, 16, "0123456789ABCDEF");
    } else {
        print_unsigned(i, 16, "0123456789abcdef");
    }
}

...
```


### Separate abstraction layers and logical steps into functions

The following fragment was shortened for the purpose of this text, because
the entire solution was inside one function.

```c
void simple_printf(const char* format, ...) {
    va_list args;
    va_start(args, format);

    const char* it = format;
    while (*it != '\0') {
        if (*it != '%') {
            putchar(*it);
        } else {
            it++;
            switch (*it) {
            case 'c': {
                int character = va_arg(args, int);
                putchar(character);
                break;
            }
            case 'd': {
                int integer = va_arg(args, int);
                ... // code for printing int
                break;
            }
            case 'x':
                int integer = va_arg(args, int);
                ... // code for printing hex (virtually copy of the above)
                break;
         ...
```

In general, each function should have a single responsibility. The above
function does too many things. Splitting the handling of the individual
specifiers (`d`, `x`, etc.) into separate functions would make the function
easier to read, comprehend and maintain. It would also enable unit testing
of the individual functions for printing primitive types.

A rule of thumb is that a function should fit on one screen. This rule dates
back to times when a screen had 25 lines in text mode (or 43 for the luckier
ones). If your function is much longer than that, there _might_ be something
wrong. Another hint is if you are unable to capture the intent of the function
in a single sentence. Do not be afraid to create single-line functions to
hide implementation details of some higher-level operation/logical step.

In this particular case, we would argue that the `simple_printf` function
itself should only take care of the top-level iteration and printing of normal
characters, delegating the handling of specifier characters to separate
dispatch function.

```c

void simple_printf(const char* format, ...) {
    va_list args;
    va_start(args, format);

    const char* format_it = format;
    while (str_has_next(format_it)) {
        char format_char = str_next(&format_it);
        if (format_char != '%') {
            putchar(format_char);
        } else if (str_has_next(format_it)) {
            format_it = format_argument(format_it, args);
        }
    }

    va_end(args);
}
```

Note that in the example above, we make a copy of the original `format` pointer
to serve as an iterator (keeping the input arguments unmodified is a good habit
on its own) and use `str_has_next` and `str_next` functions to abstract away
the pointer operations needed to iterate characters in a string.

Also note that the responsibility for updating the iterator is passed to the
`format_arg` function, which returns an updated iterator when it consumes the
specifier character. This approach can be used again when handling typed
pointers -- the `format_argument` function can print the raw pointer address
and then call `format_pointer` function to handle type-specific output. That
function would again return an updated iterator to reflect the consumed
characters. 

The implementation of `simple_printf` can be then split roughly into following
functions (with no need for other helper functions):

```c
static inline char str_next(const char** str_ptr);
static inline bool str_has_next(const char* str);

static void print_string(const char* str);
static void print_unsigned(unsigned long long value, int base, const char* alphabet);
static void print_signed(long long value, int base, const char* alphabet);

static void print_pointer(void* ptr);
static void print_buffer(simple_printf_buffer_t* buffer);
static void print_list_details(const list_t* list);

static const char* format_pointer(const char* format, const void* ptr);
static const char* format_argument(const char* format, va_list args);
```

### Avoid fusing multiple operations together

This is also related to levels abstraction, especially the inability to
separate them. Consider the following code:

```c
int freq_str_add(freq_str_t* freq, const char* str) {
    // first checking if str is already present in freq
    list_foreach(freq->list, freq_item_t, link, it) {
        if (str_equal(it->word, str)) {
            it->n++;
            return 0;
        }
    }

    // inserting a new item into the list
    ...
}

int freq_str_get(freq_str_t* freq, const char* str) {
    list_foreach(freq->list, freq_item_t, link, it) {
        if (str_equal(it->word, str)) {
            return it->n;
        }
    }
    return 0;
}
```

Both functions need to look up a word in the list of words. Both iterate over
the list, and if the word is found, they do something with it (either increment
the frequency counter, or return its value). But the way it is done makes it
difficult to extract the word lookup operation into a separate function (to
eliminate code duplication and to layer the abstractions). In addition, any
reader of this code first needs to go through the lookup implementation
details to get to the action.

The problem in the above code is that it fuses the lookup operation and the
subsequent action together. Both functions should be therefore refactored to
perform the operation as a sequence  of logical steps and the code should
reflect that sequence.

The `freq_str_add` first needs to check if a word is already present. If yes,
it should increment the word frequency, if not, it should add a new entry. This
is reflected in the following code:

```c
int freq_str_add(freq_str_t* freq, const char* str) {
    freq_item_t * item = freq_str_lookup(freq, str);
    if (item != NULL) {
        item->n++;
    } else {
        // insert a new item into the list, return error on failure
        ...
    }

    return 0;    
}
```

Similarly, the `freq_str_get` function also needs to lookup the word and
return its frequency. This is reflected in the following code:

```c
int freq_str_get(freq_str_t* freq, const char* str) {
    freq_item_t * item = freq_str_lookup(freq, str);
    return (item != NULL) ? item->n : 0;
}
```

Because both functions use the `freq_str_lookup` function, there is no need
to comment what the implementation details are supposed to do: we know that
they first do a lookup and then something else based on the result. Without
the entanglement (of the action with the lookup implementation details), we can
provide the `freq_str_lookup` function, also eliminating code duplication:

```c
freq_item_t * freq_str_lookup(freq_str_t* freq, const char* str) {
    list_foreach(freq->list, freq_item_t, link, item) {
        if (str_equal(it->word, str)) {
            return item;
        }
    }
    
    return NULL;
}
```


### Avoid state variables

```c
int freq_str_add(freq_str_t* freq, const char* str) {
    int success = -1;
    if (list_is_empty(&freq->words)){
        word_t* word = word_create(str);
        if (word != NULL){
            list_append(&freq->words, &word->link);
            success = 0;
        }
    }
    else{
        int word_exist = 0;
        list_foreach (freq->words, word_t, link, it) {
            if (strcmp(it->spell, str) == 0){
                it->count += 1;
                word_exist = 1;
                success = 0;
                break;
            }
        }
        if ( word_exist == 0 ){
            word_t* word = word_create(str);
            if (word != NULL){
                success = 0;
                list_append(&freq->words, &word->link);
            }
        }
    }
    return success;
}
```

While the above works as expected, the reader of the code needs to make sense
of three states and two extra variables. Note also that the cases when the list
is empty and when the word does not exist in the list are handled in the same
way. However, this fact is not immediately obvious and requires a bit of extra
mental processing, because the lines with `success = 0` and `list_append` are
in different order.

The code can be converted to the following form where we use early returns to
structure the logic of the function. This also decreases the indentation and
captures the logic of handling special cases first (and returning).

```c
int freq_str_add(freq_str_t* freq, const char* str) {
    list_foreach (freq->words, word_t, link, it) {
        if (strcmp(it->word, str) == 0) {
            it->count += 1;
            return 0;
        }
    }

    // Word not found.
    word_t* word = word_create(str);
    if (word == NULL) {
        return ENOMEM;
    }

    list_append(&freq->words, &word->link);
    return 0;
}
```


### Consider using asserts for preconditions

```c
int freq_str_get(freq_str_t* freq, const char* str) {
    if (str == NULL || freq == NULL)
        return -1;
    ...
}
```

We have rarely seen the defensive approach as seen above. Note that it depends
on our decision whether to return `-1` or `0` (which breaks subsequent
computations a bit less, perhaps) but the code itself states the precondition.
Another alternative would be to add an assertion instead of the condition.

```c
   assert("searched string cannot be null" && str != NULL);
   assert("frequency counter cannot be null" && freq != NULL);
```

Note that the idiom above is a simple way to insert descriptive error message
into the assertion (that the standard C version otherwise misses). [It works
because the string is an array with definitely non-zero address and therefore
evaluates to `true` and thus the outcome of the assertion depends on the second
part of the condition.]

For extra defensive code (recall that assertions work in debug builds only),
the following might work best:

```c
   assert("searched string cannot be null" && str != NULL);
   assert("frequency counter cannot be null" && freq != NULL);
   if (str == NULL || freq == NULL) {
        return 0;
   }
```


## C language bits


### `static` functions

The modifier `static` in C has several -- often unrelated -- meanings.

For variables declared inside a function it denotes that the variable has
local scope but global lifespan (this behaviour is similar to some other
languages).

When a function is marked as `static` it means that the function will not be
available to any code outside current compilation unit, which is typically
the single C source file. Therefore, `static` is a modifier that shall be
used for any helper function that shall not be used from a different source
file. Thus, all the `print_unsigned` and similar functions we mentioned
above are declared as `static`: we do not want them to be used outside
`print.c`.

Note that strict compiler settings enforce that each function is first declared
(usually in a separate header file) and then the implementation is provided.
This is not needed for `static` functions.

Always make your internal functions `static`. Not only you prevent their
invocation from unrelated code but you can also prevent surprises when the
linker declares a conflict (i.e. two helper functions have the same name and
since they are not `static`, they belong to the global identifier namespace so
to say) of completely unrelated functions.


### `va_list`

```c
void arg_print_int(va_list* args_ptr)
```

`va_list` can be passed by value, see `vfprintf` for an example
(`man 3 vfprintf`).


### Documentation comments

```c
/** Simplified printf.
 *
 * @param format Formatting string.
 * @param ... Arguments for the format string.
 */

char *convert(uintptr_t x, int base, bool small){
```

Comments starting with `/**` are considered documentation comments and can be
parsed by Doxygen (and can be used to generated HTML API documentation). Keep
them with the code they document.
 

### Machine endianess

The code you will be writing will not communicate over network. Therefore,
thinking about machine endianess is not really needed. When you want to
dump a buffer, you will simply dump it in the memory order (lower to upper
addresses) without any interpretation. And, obviously, reading integers from
memory is correct by design (i.e., the compiler knows the endianity).


### `stdout` vs `stderr`

You should print error messages to standard error output (i.e. to `&2` that you
may recall from shell scripting). In your kernels, you will have only `printk`
because kernels generally print only errors or debugging messages, but for
userspace application, the distinction is useful (and needed).

```c
printf("Error: Out of memory!"); // wrong
fprintf(stderr, "Error: out of memory (wanted %zu bytes).\n", sizeof(freq_str_t));
```


### Silencing compiler warnings

The below code was added as otherwise the compiler would complain about an
unused argument (which generally means that perhaps something is wrong with
the function).

```c
(void)freq; // Silence compiler warning
```

You can safely remove these lines (completely) once you start work on your
implementation. As a matter of fact, you _should_ remove them as otherwise they
will invalidate the purpose of the compiler check.


### Error reporting

Prefer to use error codes from `errno.h` whenever possible. And especially when
an example code uses them. Therefore, the following is not very user-friendly.

```c
if (x == NULL) {
    return 1;
}
```

Either of the following versions would work better (in our API, the first
version respects the intent, the second version is present in many POSIX APIs).

```c
if (x == NULL) {
    return ENOMEM;
}
if (x == NULL) {
    errno = ENOMEM;
    return -1;
}

```


## Memory allocations

Though the examples below are all about proper use of `malloc()`, the rules are
generally applicable to _any_ resource you need to manage. That includes
dynamically allocated memory, opened files, database cursors, etc.

And it does not really matter whether the language has exceptions or
requires using return codes for passing information about error state.

```c
el = (freq_str_el_t*) malloc(sizeof(freq_str_el_t));
el->str = str;
el->count = 0;
```

The above code misses a check for `malloc` running out of memory and returning
NULL. While it is rather unusual to run out of memory for userland programs,
you _will_ run out of memory in kernel. Please, get used to checking for that
condition (and recall the first paragraph of this section).

The code below shows a correct way to allocate a word in the frequency
counting example, including making a copy of the string itself (comments
were added by us).

```c
data_block_t* fs = (data_block_t*) malloc(sizeof(data_block_t));
if (fs == NULL) {
    return ENOMEM;
}

fs->word = strdup(str); // this calls malloc somewhere inside too
if (fs->word == NULL) { // and thus has to be checked as well
    free(fs);           // on failure, we release previously allocated resource
    return ENOMEM;
}

// No failures below
fs->count = 1;
link_init(&fs->link);
list_append(&freq->list, &fs->link);
```

On a C-language note, the type-cast in front of `malloc` is not needed.
`malloc` returns `void *` that can be cast to any type implicitly.
But it is completely fine to leave it there too.

Note that if you allocate words, you should also free them. Therefore, the
following implementation is incomplete (every `malloc` should have a `free`
counterpart).

```c
void freq_str_destroy(freq_str_t* freq) {
    free(freq);
}
```

Note that the above leaks memory, because it does not free the allocated words.
Even though memory is not as scarce as it used to be, there are other resources
that you can leak in a similar manner, such as file descriptors (if you forget
to close files) or database connections. If your code runs long enough, 
resource leaks will eventually get you.


## Other notes

Bits that did not fit anywhere else.


### Needless checks for list emptiness

```c
if (!list_is_empty(&list)) {
   list_foreach(...) {
      ...
      if (...) {
          return ...;
      }
   }
}
```

The check for list emptiness is not really needed. The `list_foreach` function
works fine even with empty lists -- the body of the loop will be never executed.
Not a big problem, though.


### Purpose of the assignment I

```c
case 'p': {void * ptr = va_arg(args,void *);
                            sprintf(buf,"%p",ptr);
                            fputs(buf,stdout);
                            break;}
```

We hope that the above fragment was caused by misunderstanding of the purpose
of the assignment as it borders on cheating. Please, always consider what is
the intent of the assignment and if unsure, ask us.

Reusing existing code is fine unless it contains the essence of the assignment
(e.g., assignment about heap allocator can easily reuse red-black trees from
Linux but should not reuse code from Doug Lea allocator).


### Purpose of the assignment II

If the assignment states that

> _the sole purpose_ of this exercise is to let you learn how to use the
> provided linked list implementation

we do not expect you to submit any of the following code:

```c
typedef struct {
    char* string;
    int count;
} str_t;

struct freq_str {
    freq_str_t* nextNode;
    str_t* dataNode;
};
```

```c
typedef struct freq_str {
    struct freq_str* next;
    const char* s;
} freq_str_t;
```

If you were unsure about the use of lists, you should have asked.

This also covers rather unexpected implementations that contained the following
fragments. We expected you to use `list_t` as a library instead of poking
inside (implementing support for `%pL` being an obvious exception to that
expectation).

```c
static inline link_t* freq_str_pop(freq_str_t* list) {
    assert(list != NULL);
    if (freq_str_empty(list))
        return NULL;
    link_t* item = list->head.next;
    list_remove(item);
    return item;
}
```

```c
void freq_str_destroy(freq_str_t* freq) {
    link_t* head = &freq->list.head;
    link_t* link = head->next;

    while (link != head) {
        link_t* next = link->next;
        free(list_item(link, strcnt_t, link));
        link = next;
    }
    free(freq);
}
```

Note that a proper implementation of `freq_str_destroy` can look like this
(this comes from another implementation but it covers the typical case).

```c
while (!list_is_empty(&freq->words)) {
    link_t* first = list_pop(&freq->words);
    freq_t* word = list_item(first, freq_t, link);
    free(word);
}
free(freq);
```
