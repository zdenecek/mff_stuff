package org.example;

import java.util.Set;

public interface Employees  {
    int add(String name, int salary);
    Set<Integer> getAll();
    String getName(int id);
    int getSalary(int id);
    void changeSalary(int id, int newSalary);
}
