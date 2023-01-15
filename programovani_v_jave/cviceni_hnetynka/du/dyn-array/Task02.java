package com.mypackage;

import java.util.Iterator;
import java.util.NoSuchElementException;

public class Task02 {
    public static void main(String[] args) {

        var array = new DynamicArray();

        for(var arg : args) {
            array.add(arg);
        }
        for(var arg : array) {
            System.out.println(arg);
        }
    }
}

interface MyCollection {
    void add(Object o);
    Object get(int i);
    void remove(Object o);
    void remove(int i);
}

class DynamicArray implements MyCollection, Iterable {

    private int capacity;
    private int filled_capacity;
    private Object[] data;

    DynamicArray( int initial_capacity ) {
        if( initial_capacity <= 0 ) {
            throw new IllegalArgumentException("Initial capacity must be greater than 0");
        }
        filled_capacity = 0;
        capacity = initial_capacity;
        data = new Object[capacity];
    }

    DynamicArray() {
        this(4);
    }

    public int getLength() {
        return filled_capacity;
    }

    @Override
    public void add(Object o) {
        if(capacity == filled_capacity) expand();
        data[filled_capacity] = o;
        filled_capacity++;
    }

    @Override
    public Object get(int i) {
        if(i >= filled_capacity) throw new IndexOutOfBoundsException("Index out of bounds");
        return data[i];
    }

    @Override
    public void remove(Object o) {
        for(int i = 0; i < filled_capacity; i++) {
            if(data[i].equals(o)) {
                remove(i);
                return;
            }
        }
    }

    @Override
    public void remove(int i) {
        if(i >= filled_capacity) throw new IndexOutOfBoundsException("Index out of bounds");
        data[i] = null;
        for(int j = i; j < filled_capacity; j++) {
            data[j] = data[j+1];
        }
        filled_capacity--;
        if(filled_capacity < capacity/3) shrink();
    }

    private void expand() {
        var lastData = data;
        capacity *= 2;
        data = new Object[capacity];
        copyFrom(lastData, filled_capacity);
    }

    private void shrink() {
        // not required
    }

    private void copyFrom(Object[] src, int count) {
        for(int i = 0; i < count; i++) {
            data[i] = src[i];
        }
    }

    @Override
    public Iterator iterator() {
        return new DynamicArrayIterator(this);
    }
}

class DynamicArrayIterator implements Iterator {

    private int index = -1;
    private DynamicArray array;

    DynamicArrayIterator(DynamicArray array) {
        this.array = array;
    }

    @Override
    public boolean hasNext() {
        return array.getLength() > index + 1;
    }

    @Override
    public Object next() {
        if(hasNext()) {
            index++;
            return array.get(index);
        }
        throw new NoSuchElementException("No more elements");
    }
}
