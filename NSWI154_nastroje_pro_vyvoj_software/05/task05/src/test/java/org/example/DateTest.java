package org.example;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;
import org.junit.jupiter.params.provider.ValueSource;

import static org.junit.jupiter.api.Assertions.*;

import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.stream.Stream;

class DateTest {

    //  constructor that accepts "String pattern", the method "String format(Date)", and the method "Date parse(String)".
    //  Note that methods "format" and "parse" are defined in the superclass.
    //   Use advanced features of JUnit, respectively the framework of your choice for other languages,
    //   especially parameterized tests.


    @ParameterizedTest
    @ValueSource(strings = { "yyyy-MM-dd", "yyyy-MM-dd HH:mm:ss", "yyyy-MM-dd HH:mm:ss.SSS" })
    void testFormat(String candidate) {
        SimpleDateFormat sdf = new SimpleDateFormat(candidate);
        String date = "2020-01-01 12:00:00.000";
        try {
            assertEquals(date, sdf.format(sdf.parse(date)), "Date should be formatted correctly");
        } catch (Exception e) {
            fail("Exception thrown");
        }
    }

private static Stream<Arguments> provideParameters() {
    return Stream.of(
            Arguments.of("dd/MM/YYYY", "01/01/2020"),
            Arguments.of("yyyy-MM-dd HH:mm:ss", "2020-01-01 12:00:00"),
            Arguments.of("yyyy-MM-dd HH:mm:ss.SSS",    "2020-01-01 12:00:00.000")
    );
}

    @ParameterizedTest
    @MethodSource("provideParameters")
    void testParse(String format, String expected ) {
        SimpleDateFormat sdf = new SimpleDateFormat(format);

        try {
            assertEquals(expected, sdf.format(new Date(2020,1,1,12,0,0)), "String format should match given formatting");
        } catch (Exception e) {
            fail("Exception thrown");
        }
    }
}