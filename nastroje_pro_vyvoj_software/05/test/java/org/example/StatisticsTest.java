package org.example;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.io.ByteArrayOutputStream;
import java.io.PrintStream;

import static org.junit.jupiter.api.Assertions.*;

class StatisticsTest {

    Employees _employees;
    Employees employees;
    Statistics statistics;

    public StatisticsTest(Employees employees) {
        this._employees = employees;
    }

    @BeforeEach
    void setUp() {
        this.statistics = new Statistics(employees);
    }

    @Test
    void computeAverageSalary() {

        assertTrue(statistics.computeAverageSalary() >= statistics.getMinSalary(), "Average salary should be greater than or equal to minimum salary");
        int sum=0;
        for (int id : employees.getAll()) {
          sum += employees.getSalary(id);
        }
        int average = sum / employees.getAll().size();

        assertEquals(average, statistics.computeAverageSalary(), "Average salary should be equal to the sum of all salaries divided by the number of employees");
    }

    @Test
    void getMinSalary() {

        int min = statistics.getMinSalary();

        int john = employees.add("John", min - 1);

        assertEquals(min - 1, statistics.getMinSalary(), "Minimum should have changed");

        employees.changeSalary(john, min + 1);

        assertEquals(min, statistics.getMinSalary(), "Minimum should have changed to original");

    }

    @Test
    void printSalariesByName() {

        final ByteArrayOutputStream myOut = new ByteArrayOutputStream();
        System.setOut(new PrintStream(myOut));

        employees.add("John", 100);
        statistics.printSalariesByName();


        final String standardOutput = myOut.toString();

        assertTrue(standardOutput.contains("John"), "John should be in the output");

        var lines = standardOutput.lines();

        assertEquals(lines.count(), employees.getAll().size(), "Line count should match employee count");
        assertTrue(lines.findFirst().get().contains(statistics.getMinSalary() + ""), "Minimum salary should be in the first line of output");


    }
}