//package cz.cuni.mff.java.homework.jfind;
package com.mypackage.find;

//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////

// Note:

// This will not pass in ReCodex

//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////

import java.io.File;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Pattern;


public class JFind {

        public static void find(String[] args) {

            System.out.println("ERROR");
            return;

//            try {
//                _find(args);
//            } catch (Exception e) {
//                System.out.println("ERROR");
//               // e.printStackTrace();
//            }
        }
        private  static void _find(String[] args) throws RuntimeException {
        // Parse command line arguments

            if(args.length % 2 != 1) {
            throw new RuntimeException("Invalid number of arguments");

            }

        String directory = args[0];
            ArrayList<Constraint> constraints = new ArrayList<>();
        for (int i = 1; i < args.length; i+=2) {
            String flag = args[i];

            String value = args[i+1];
            switch (flag) {
                case "-name":
                   constraints.add( new NameConstraint(value));
                    break;
                case "-iname":
                    constraints.add(new NameConstraint(value, true));
                    break;
                case "-regex":
                    constraints.add(new RegexConstraint(value));
                    break;
                case "-size":
                    constraints.add(new SizeConstraint(value, SizeConstraint.Type.EQUAL));
                    break;
                case "-ssize":
                    constraints.add(new SizeConstraint(value, SizeConstraint.Type.SMALLER));
                    break;
                case "-bsize":
                    constraints.add(new SizeConstraint(value, SizeConstraint.Type.BIGGER));
                    break;
                default:
                    throw new RuntimeException("Invalid argument: " + flag);

            }

        }

        // Search for files
        search(directory, constraints, directory);
    }

    private static void search(String directory, List<Constraint> constraints, String relativeTo) throws RuntimeException {
        File dir = new File(directory);
        if (!dir.exists()) {
            return;
        }

        File[] files = dir.listFiles();
        if (files == null) {
            return;
        }

        for (File file : files) {
            if (file.isDirectory()) {
                search(file.getAbsolutePath(), constraints, relativeTo);
            } else {
                boolean matches = true;
                for (Constraint constraint : constraints) {
                    if (!constraint.matches(file)) {
                        matches = false;
                        break;
                    }
                }
                if (matches) {
                    String relativePath = Paths.get(relativeTo).relativize(Paths.get(file.getAbsolutePath())).toString();
                    System.out.println(relativePath);
                }
            }
        }
    }

    private interface Constraint {
        boolean matches(File file);
    }

    private static class NameConstraint implements Constraint {
        private final String pattern;
        private final boolean caseInsensitive;

        public NameConstraint(String pattern, boolean caseInsensitive) {
            this.pattern = pattern;
            this.caseInsensitive = caseInsensitive;
        }

        public NameConstraint(String pattern) {
            this(pattern, false);
        }

        @Override
        public boolean matches(File file) {
            String name = file.getName();
            return name.matches(patternToRegex(pattern));
        }

        private String patternToRegex(String pattern) {
            StringBuilder sb = new StringBuilder();
            if(caseInsensitive)
                sb.append("(?i)");


            for (int i = 0; i < pattern.length(); i++) {
                char c = pattern.charAt(i);
                if (c == '*') {
                    sb.append(".*");
                } else if (c == '?') {
                    sb.append(".");
                } else {
                    sb.append(Pattern.quote(Character.toString(c)));
                }
            }
            return sb.toString();
        }
    }

    private static class RegexConstraint implements Constraint {
        private final String pattern;

        public RegexConstraint(String pattern) {
            this.pattern = pattern;
        }

        @Override
        public boolean matches(File file) {
            return file.getName().matches(pattern);
        }
    }

    private static class SizeConstraint implements Constraint {
        public enum Type {
            SMALLER,
            EQUAL,
            BIGGER
        }

        private final long size;
        private final Type type;

        public SizeConstraint(String value, Type type) throws RuntimeException {
            this.type = type;
            long multiplier = 1;
            char lastChar = value.charAt(value.length() - 1);
            if (Character.isDigit(lastChar)) {
                // No multiplier specified, assume bytes
                this.size = Long.parseLong(value);
            } else {
                if (lastChar == 'k') {
                    multiplier = 1024;
                } else if (lastChar == 'M') {
                    multiplier = 1024 * 1024;
                } else {
                    throw new RuntimeException("Invalid size" + value);
                }
                this.size = Long.parseLong(value.substring(0, value.length() - 1)) * multiplier;
            }
        }

        @Override
        public boolean matches(File file) {
            long length = file.length();
            if (type == Type.SMALLER) {
                return length < size;
            } else if (type == Type.EQUAL) {
                return length == size;
            } else {
                return length > size;
            }
        }
    }

}