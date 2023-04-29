package com.mypackage.Brainfuck;
import java.io.*;
import java.util.*;

public class Brainfuck {
    // Constants for the default memory array size and the ASCII values of the
    // BrainF*** commands
    private static final int DEFAULT_MEMORY_SIZE = 30000;
    private static final char INC_DP = '>';
    private static final char DEC_DP = '<';
    private static final char INC_CELL = '+';
    private static final char DEC_CELL = '-';
    private static final char PRINT_CELL = '.';
    private static final char INPUT_CELL = ',';
    private static final char JMP_FWD = '[';
    private static final char JMP_BWD = ']';


    public static void main(String[] args) {
        // Parse the command line arguments
        String programFile = args[0];
        int memorySize = DEFAULT_MEMORY_SIZE;
        if (args.length > 1) {
            memorySize = Integer.parseInt(args[1]);
        }

        // Read the BrainF*** program from the file
        Program program = readProgram(programFile);

        if(program.program == "") {
            return;
        }

        // Initialize the memory array and data pointer
        byte[] memory = new byte[memorySize];
        Arrays.fill(memory, (byte)0);

        int dp = 0;

        var l = program.program.length();
        // Iterate through the BrainF*** program, one character at a time
        for (int i = 0; i < l; i++) {
            char c = program.program.charAt(i);
            switch (c) {
                case INC_DP:
                    // Increment the data pointer
                    dp++;
                    break;
                case DEC_DP:
                    // Decrement the data pointer
                    dp--;
                    break;
                case INC_CELL:
                    // Increment the value of the cell at the data pointer
                    memory[dp]++;
                    break;
                case DEC_CELL:
                    // Decrement the value of the cell at the data pointer
                    memory[dp]--;
                    break;
                case PRINT_CELL:
                    // Print out the value of the cell at the data pointer as a Unicode character
                    System.out.print((char)memory[dp]);
                    break;
                case INPUT_CELL:
                    // Accept a value from the input and store it in the cell at the data pointer
                    try {
                        memory[dp] = (byte)System.in.read();
                    } catch (IOException e) {
                        System.err.println("Error reading input: " + e.getMessage());
                    }
                    break;
                case JMP_FWD:

                    // If the value of the cell at the data pointer is equal to zero, jump to the command after the matching ']'
                    if (memory[dp] == 0) {
                        i = program.bracketPairs.get(i);

                    }

                    break;
                case JMP_BWD:
                    // If the value of the cell at the data pointer is non-zero, jump to the command before the matching '['
                    if (memory[dp] != 0) {
                        i = program.bracketPairs.get(i);
                    }
                    break;
            }

            // Check if the data pointer is within the bounds of the memory array
            if (dp < 0 || dp >= memorySize) {
                System.out.println(dp < 0 ? "Memory underrun" : "Memory overrun");
                return;
            }
        }
    }
    // Helper function to read the BrainF*** program from a file and check for unopened or unclosed cycles


    private static Program readProgram(String fileName) {

        int resChar = 0;
        Stack<Integer> openBracketsRes = new Stack<>();
        HashMap<Integer, Integer> bracketPairsRes = new HashMap<>();

        StringBuilder sb = new StringBuilder();
        Stack<Integer> openBrackets = new Stack<>();
        Stack<Integer> lines = new Stack<>();
        int l = 1;
        int charPos = 1;
        try (BufferedReader br = new BufferedReader(new FileReader(fileName))) {
            String line;
            while ((line = br.readLine()) != null) {
                for (int i = 0; i < line.length(); i++) {
                    char c = line.charAt(i);
                    if (c == JMP_FWD) {
                        // Push the position of the open bracket onto the stack
                        openBrackets.push(charPos);
                        lines.push(l);
                        openBracketsRes.push(resChar);
                    } else if (c == JMP_BWD) {
                        // Pop the top element from the stack and check if it is a matching open bracket
                        if (openBrackets.isEmpty()) {
                            System.out.println("Unopened cycle - line " + l + " character " + charPos);
                            return new Program("", bracketPairsRes);
                        }
                        var open = openBracketsRes.pop();
                        bracketPairsRes.put(open , resChar );
                        bracketPairsRes.put(resChar, open );
                        openBrackets.pop();
                    }
                    charPos++;
                    resChar++;
                }
                l++;
                charPos = 1;
                sb.append(line);
            }
        } catch (IOException e) {
            System.out.println("Error reading program file: " + e.getMessage());
        }
        // Check if there are any unclosed cycles
        if (!openBrackets.isEmpty()) {
            while(!openBrackets.isEmpty()) {
                System.out.println("Unclosed cycle - line " + lines.pop() + " character " + openBrackets.pop());
            }
            return new Program("", bracketPairsRes);
        }
        return new Program(sb.toString(), bracketPairsRes);
    }

}
class Program {
    String program;

    public Program(String program, HashMap<Integer, Integer> bracketPairs) {
        this.program = program;
        this.bracketPairs = bracketPairs;
    }

    HashMap<Integer, Integer> bracketPairs;
}