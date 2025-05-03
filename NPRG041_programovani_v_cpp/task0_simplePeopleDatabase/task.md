Create a simple people database which supports these operations: `print`, `insert`, `remove`, `sort` and `exit`. Every people has a `name` and an `age`. You can sort/remove by one of those attributes.

# Input

## Commands

### Exit: `exit`

-   exits the program
-   format:

```
exit
```

### Insert: `insert`

-   insert a new person into the database
-   format:

```
insert
<name> <age>
```

-   `<name>`: name of the person (doesn't contain spaces)
-   `<age>`: age of the person.

### Printing: `print`

-   prints the database
-   each entry on a single line
-   input format:

```
print
```

-   output format:

```
{ name: <name>, age: <age> }
```

-   `<name>`: name of the person
-   `<age>`: age of the person

### Sorting

-   Sorting is stable

#### Sorting by age: `sort_age`

-   Sorts persons by their age
-   format:

```
sort_age
```

#### Sorting by name: `sort_name`

-   format:

```
sort_name
```

### Removing

#### Removing by age: `remove_age`

-   Remove **all** entries with the given age
-   format:

```
remove_age
<age>
```

-   `<age>`: age of persons which should be removed

#### Removing by name: `remove_name`

-   Remove **all** entries with the given name
-   format:

```
remove_name
<name>
```

-   `<name>`: name of persons which should be removed

# Comments/hints

-   Ordering without sorting is according the insertion
-   Multiple entries with the same name and age are allowed
-   Remove functions remove **all** entries with the given name/age
-   Check `<algorithm>` for useful functions
-   Test your program using input redirection from file, e.g., `my_prg < input_file`

# Example

## Input

```
insert
adolf 10
insert
blanca 9
insert
c 8
insert
dan 7
sort_age
print
remove_name
blanca
print
exit
```

## Output

```
{ name: dan, age: 7 }
{ name: c, age: 8 }
{ name: blanca, age: 9 }
{ name: adolf, age: 10 }
{ name: dan, age: 7 }
{ name: c, age: 8 }
{ name: adolf, age: 10 }
```