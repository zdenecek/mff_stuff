package com.mypackage.stest;

import java.io.IOException;
import java.util.*;
import java.util.stream.Collectors;

public class ExamValidator {

    public static void main(String[] args) throws IOException {
        var validator = new ExamValidator(new Scanner(System.in));
        validator.run();
    }

    Map<Integer, ExamQuestion> questions = new HashMap<>();
    Scanner in;
    int lineNumber = 0;

    private void printError() {
        System.out.println("ERROR - line " + lineNumber);
    }

    private String nextLine() {
        lineNumber++;
        return in.nextLine().trim();
    }

    public ExamValidator(Scanner in) {
        this.in = in;
    }

    void parseQuestion(String questionType) {

        ExamQuestion.Type type = questionType.equals("multichoice") ? ExamQuestion.Type.MULTICHOICE : ExamQuestion.Type.SINGLECHOICE;


        String line = nextLine();
        String[] parts = line.split("\\s+", 2);
        int questionNum = Integer.parseInt(parts[0].replace(".", ""));
        String questionText = parts[1];

        line = nextLine();

        while (!(line.length() >= 2 && 'A' <= line.charAt(0) && line.charAt(0) <= 'Z' && line.charAt(1) == '.')) {
            questionText += " " + line;
            line = nextLine();
        }

        ExamQuestion question = new ExamQuestion(type, questionNum, questionText);

        String answerText = "";
        char answerLetter = 'A';
        while (!line.startsWith("Answer:")) {

            if (line.length() >= 2 && 'A' <= line.charAt(0) && line.charAt(0) <= 'Z' && line.charAt(1) == '.') {

                if (!answerText.equals("")) question.addAnswer(answerLetter, answerText);

                parts = line.split("\\s+", 2);
                answerLetter = parts[0].charAt(0);
                answerText = parts[1];
            } else {
                answerText += line;
            }

            line = nextLine();
        }

        question.addAnswer(answerLetter, answerText);

        line = line.replace("Answer:", "").replace(" ", "");

        var answers = line.chars().mapToObj(e -> (char) e).collect(Collectors.toList());

        if(answers.stream().anyMatch(c -> !question.answers.containsKey(c))) throw new RuntimeException("Invalid correct answer");
        question.correctAnswers.addAll(answers);
        questions.put(questionNum, question);
    }



    ExamAnswer parseAnswer(String name) {

        ExamAnswer examAnswer = new ExamAnswer(name);
        String line;
        while (in.hasNextLine()) {
            line = nextLine();
            if (line.isEmpty()) break;

            String[] parts = line.split("\\s+", 2);
            if (parts.length < 2) {
                throw new RuntimeException("Invalid answer");
            }
            int id = Integer.parseInt(parts[0].replace(".", ""));
            var answers = parts[1].replace(" ", "").chars().mapToObj(e -> (char) e).collect(Collectors.toSet());


            examAnswer.addAnswer(id, answers);
        }
        return examAnswer;
    }

    public void run() throws IOException {
        boolean skipBlock = false;
        String line = "";
        while (in.hasNextLine()) {
            line = nextLine();

            if (!line.isEmpty() && skipBlock) {
                continue;
            }
            skipBlock = false;

            if (line.isEmpty()) {
                continue;
            }

            if (line.equals("multichoice") || line.equals("singlechoice")) {

                try {

                    parseQuestion(line);
                } catch (Exception e) {
                    printError();
                    skipBlock = true;
                    continue;
                }

            } else {
                break;
            }
        }
        boolean first = true;

        while (in.hasNextLine()) {

            if (!first) {
                line = nextLine();
            }

            if (!line.isEmpty() && skipBlock) {
                continue;
            }
            skipBlock = false;

            try {
                var answer = parseAnswer(line);
                if (first) first = false;
                else System.out.println();
                evalAnswer(answer);
            } catch (Exception e) {
                printError();
                skipBlock = true;
            }
        }
    }

    void evalAnswer(ExamAnswer examAnswer) {
        int totalPoints = 0;
        System.out.println(examAnswer.name);
        for (Map.Entry<Integer, ExamQuestion> entry : questions.entrySet()) {
            int id = entry.getKey();
            ExamQuestion question = entry.getValue();
            Set<Character> correctAnswers = question.correctAnswers;
            Set<Character> givenAnswers = examAnswer.answers.get(question);
            if (givenAnswers == null) {
                givenAnswers = Collections.emptySet();
            }
            int points = 0;

            for (char c : correctAnswers) {
                if (!givenAnswers.contains(c)) {
                    points++;
                }
            }
            for (char c : givenAnswers) {
                if (!correctAnswers.contains(c)) {
                    points++;
                }
            }

            totalPoints += points;
            System.out.println(id + ". " + points);
        }
        String result;
        if (totalPoints >= 9) {
            result = "4";
        } else if (totalPoints >= 6) {
            result = "3";
        } else if (totalPoints >= 3) {
            result = "2";
        } else {
            result = "1";
        }
        System.out.println("Result: " + result + " (" + totalPoints + ")");

    }

    class ExamQuestion {
        Type type;
        int id;
        String text;
        Map<Character, String> answers = new HashMap<>();
        Set<Character> correctAnswers = new HashSet<>();
        ExamQuestion(Type type, int id, String text) {
            this.type = type;
            this.id = id;
            this.text = text;
        }

        void addAnswer(char id, String text) {

            answers.put(id, text);
            if (this.type == Type.SINGLECHOICE && correctAnswers.size() > 1) {
                throw new RuntimeException("Single choice question can have only one answer");
            }
        }

        public enum Type {
            MULTICHOICE, SINGLECHOICE
        }


    }

    class ExamAnswer {
        String name;
        Map<ExamQuestion, Set<Character>> answers = new HashMap<>();

        ExamAnswer(String name) {
            this.name = name;
        }

        void addAnswer(int id, Set<Character> answers) {

            if(!questions.containsKey(id)) return; // ignored

            var question = questions.get(id);
            var possible = question.answers;
            if( answers.stream().anyMatch(a -> !possible.containsKey(a))) throw new RuntimeException("Invalid question answer in answer");

            this.answers.put(question, answers);
        }
    }
}