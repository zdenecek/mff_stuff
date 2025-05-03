package org.example;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.util.TreeMap;

import static org.junit.jupiter.api.Assertions.*;


class TreeTest {

    TreeMap<String, String> map;

    @BeforeEach
    void setUp() {
        map = new TreeMap<>();
    }

//     - adding new mapping with the key different from all present mappings
    @Test
    void testAddingNewMapping() {
        map.put("Hello", "One");
        map.put("Hullo", "Two");
        map.put("Heya", "Three");
        map.put("Ahoy", "Four");

        assertEquals(4, map.size()) ;
        assertTrue(map.containsKey("Hello"), "Map should contain key Hello");
        assertFalse(map.containsValue("Hullo"), "Map should not contain value Hello");
        assertTrue(map.containsKey("Hullo"), "Map should contain key Hello");
        assertFalse(map.containsKey("Three"), "Map should not contain key Three");
    }

//     - adding new mapping with a key equal to some already present mapping

    @Test
    void testAddingExistingMapping() {
        map.put("Hello", "One");
        map.put("Hello", "Two");

        assertEquals(1, map.size(), "Map should contain one element");
        assertTrue(map.containsKey("Hello"), "Map should contain key Hello");
        assertTrue(map.containsValue("Two"), "Map should not contain key Three");
        assertFalse(map.containsValue("One"), "Map should not contain key Three");
        assertEquals("Two", map.get("Hello"), "Map should contain key Hello");
        assertNull(map.get("Two"), "Map should not contain key Hello");

    }

    //     - removing some element (mapping) already present in the map
    @Test
    void removeExistingMapping() {
        map.put("Hello", "One");
        map.put("Hullo", "Two");

        map.remove("Hullo");

        assertEquals(1, map.size(), "Map should contain one element");
        assertTrue(map.containsKey("Hello"), "Map should contain key Hello");
        assertFalse(map.containsKey("Hullo"), "Map should not contain value Hello");
        assertFalse(map.containsValue("Two"), "Map should not contain key Three");
        assertTrue(map.containsValue("One"), "Map should not contain key Three");

    }


    //     - removing all elements from the map and checking for emptiness
    @Test
    void clearMappings() {
        map.put("Hello", "One");
        map.put("Hullo", "Two");
        map.put("Heya", "Three");
        map.put("Ahoy", "Four");


        map.clear();

        assertEquals(0, map.size(), "Map should contain no element");
        assertFalse(map.containsKey("Hullo"), "Map should not contain value Hullo");
        assertFalse(map.containsValue("Two"), "Map should not contain key Two");

    }


//     - getting the value associated with a specific key
    @Test
    void getValue() {
        map.put("Hello", "One");
        map.put("Hullo", "Two");

        assertEquals("One", map.get("Hello"), "Map should contain key Hello");
        assertEquals("Two", map.get("Hullo"), "Map should contain key Hullo");
        assertNull(map.get("Two"), "Map should not contain key Hello");

    }

//     - attempt to get a value for a key not present in the map
    @Test
    void getValueForNonExistingKey() {
        map.put("Hello", "One");
        map.put("Hullo", "Two");
        map.put("Heya", "Three");
        map.put("Ahoy", "Four");

        assertNull(map.get("Hi"), "Map should not contain key Hi");
        assertNull(map.get("Two"), "Map should not contain key Two");
    }


    @Test
    void checkNonexistentMapping() {
        map.put("Hello", "One");
        map.put("Hullo", "Two");
        map.put("Heya", "Three");
        map.put("Ahoy", "Four");

        assertTrue(map.containsKey("Hi"), "Map should contain key Hi");
    }

}

