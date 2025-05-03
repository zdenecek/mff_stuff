package org.example;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;

public class Statistics {
    private final Employees employees;

    public Statistics(Employees employees) {
        this.employees = employees;
    }

    int computeAverageSalary() {
        ArrayList<Pair<Integer, Integer>> list = new ArrayList<>();
        int sum = 0;
        for (int id : employees.getAll()) {
            sum += employees.getSalary(id);
        }

        return sum / list.size();
    }
    int getMinSalary() {
        int min = Integer.MAX_VALUE;
        for (int id : employees.getAll()) {
            int salary = employees.getSalary(id);
            if (salary < min) {
                min = salary;
            }
        }
        return min;
    }
    void printSalariesByName() // prints the list of pairs <name, salary> that is sorted by employee name
    {
        ArrayList<Pair<String, Integer>> list = new ArrayList<>();
        for (int id : employees.getAll()) {
            list.add(new Pair<>(employees.getName(id), employees.getSalary(id)));
        }
        Collections.sort(list, new Comparator<Pair<String, Integer>>() {
            @Override
            public int compare(Pair<String, Integer> o1, Pair<String, Integer> o2) {
                return o1.getKey().compareTo(o2.getKey());
            }
        });
        for (Pair<String, Integer> pair : list) {
            System.out.println(pair.getKey() + " " + pair.getValue());
        }
    }
}
