// 2024/2025
// TASK Implement the processNumbers() method sketched bellow.
// The user provides a list of operations and a collection of numbers that are supposed to be filtered and transformed 
// using the operations. The operations are of two kinds - filter and transform. They must be processed in the same order
// in which they are provided in the list.
// The filter operations calculate a logical value indicating whether a number should be preserved.
// The transformation operations compute a value that should replace the number out of which it was calculated.
// These operations correspond to Groovy's 'findAll()' and 'collect()', respectively.

// The operations are provided in its source (aka textual) form, so they must be compiled with 'GroovyShell' before they can be run.
// Some of the operations refer to a 'LENGTH' value, which represents the ORIGINAL size of the provided collection of numbers.
// The processNumbers() method must ensure that the 'LENGTH' identifier refers to the right value.

// Use some of the tricks we've learnt about scripting, like passing parameters to GroovyShell through binding,
// properties, closure delegates, the 'object.with(Closure)' method, etc.

import groovy.lang.GroovyShell

List processNumbers(List<Operation> userInput, List numbers) {

    def currentNumbers = numbers
    final LENGTH = numbers.size()
    
    final binding = new Binding()
    binding.setVariable('LENGTH', LENGTH)
    final shell = new GroovyShell(binding)

    userInput.each { operation ->
        final command = operation.command
        final closure = shell.evaluate("return $command")
        
        if (operation.type == OperationType.FILTER) {
            currentNumbers = currentNumbers.findAll { closure(it) }
        } else if (operation.type == OperationType.TRANSFORMATION) {
            currentNumbers = currentNumbers.collect { closure(it) }
        }
    }
    return currentNumbers
}

//************* Do not modify after this point!
enum OperationType {FILTER, TRANSFORMATION}

@groovy.transform.Immutable class Operation {
    final OperationType type
    final String command
}

final numbers = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 23, 27, 101, 208, 513, 1023]

//A test user script input.
List<Operation> userInput = [
                            new Operation(OperationType.FILTER, '{it > 12 || it < 5}'),
                            new Operation(OperationType.FILTER, '{it % 2 == 1}'),                            
                            new Operation(OperationType.TRANSFORMATION, '{2 * it}'),
                            new Operation(OperationType.FILTER, '{it % 5 != 0}'),   
                            new Operation(OperationType.TRANSFORMATION, '{it + LENGTH}')                                                                                 
                            ]

def result = processNumbers(userInput, numbers)
println result
assert result == [29, 33, 53, 61, 65, 69, 73, 81, 229, 1053, 2073]
println 'ok'