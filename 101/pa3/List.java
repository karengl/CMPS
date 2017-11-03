// Karen Lee
// kglee
// pa3
// List.java

public class List {
    private class Node {
        Object data;
        Node prev;
        Node next;

        Node (Object data, Node prev, Node next) {
            this.data = data;
            this.prev = prev;
            this.next = next;
        }

        public String toString() {
            return data.toString();
        }

        public boolean equals(Object x) {
            Node N;
            boolean eq = false;
            if (x instanceof Node) {
                N = (Node) x;
                eq = this.data.equals(N.data);
            }
            return eq;
        }
    }

    private Node front;
    private Node back;
    private Node cursor;
    private int length;
    private int index;

    // Creates a new empty list.
    List() {
        front = null;
        back = null;
        cursor = null;
        length = 0;
        index = -1;
    }

    // Returns the number of elements in this List.
    int length() {
        return length;
    }

    // If cursor is defined, returns the index of the cursor element,
    // otherwise returns -1.
    int index() {
        return index;
    }

    // Returns front element. Pre: length()>0
    Object front() {
        if (length < 1)
            throw new RuntimeException("List Error: front() called on empty List");
        return front.data;
    }

    // Returns back element. Pre: length()>0
    Object back() {
        if (length < 1)
            throw new RuntimeException("List Error: back() called on empty List");
        return back.data;
    }

    // Returns cursor element. Pre: length()>0, index()>=0
    Object get() {
        if (length < 1)
            throw new RuntimeException("List Error: get() called on empty List");
        if (index < 0)
            throw new RuntimeException("List Error: get() called on undefined index");
        return cursor.data;
    }

    // Returns true if this List and L are the same integer
    // sequence. The cursor is ignored in both lists.
    boolean equals(List L) {
        if (length != L.length)
            return false;
        Node Ltmp = L.front;
        Node tmp = front;
        while (tmp != null && Ltmp != null) {
            if(!(tmp.equals(Ltmp)))
                return false;
            Ltmp = Ltmp.next;
            tmp = tmp.next;
        }
        return true;
    }

    // Resets this List to its original empty state.
    void clear() {
        front = null;
        back = null;
        cursor = null;
        length = 0;
        index = -1;
    }

    // If List is non-empty, places the cursor under the front element,
    // otherwise does nothing.
    void moveFront() {
        if (length > 0) {
            cursor = front;
            index = 0;
        }
    }

    // If List is non-empty, places the cursor under the back element,
    // otherwise does nothing.
    void moveBack() {
        if (length > 0) {
            cursor = back;
            index = length - 1;
        }
    }

    // If cursor is defined and not at front, moves cursor one step toward
    // front of this List, if cursor is defined and at front, cursor becomes
    // undefined, if cursor is undefined does nothing.
    void movePrev() {
        if (cursor != null) {
            if (index != 0) {
                cursor = cursor.prev;
                index--;
            }else {
                cursor = null;
                index = -1;
            }
        }
    }

    // If cursor is defined and not at back, moves cursor one step toward
    // back of this List, if cursor is defined and at back, cursor becomes
    // undefined, if cursor is undefined does nothing.
    void moveNext() {
        if (cursor != null) {
            if (index != length-1) {
                cursor = cursor.next;
                index++;
            } else {
                cursor = null;
                index = -1;
            }
        }
    }

    // Insert new element into this List. If List is non-empty,
    // insertion takes place before first element.
    void prepend(Object data) {
        Node tmp = new Node(data, null, front);
        if (length == 0)
            back = tmp;
        else 
            front.prev = tmp;
        front = tmp;
        length++;
    }

    // Insert new element into this List. If List is non-empty,
    // insertion takes place after back element.
    void append(Object data) {
        Node tmp = new Node(data, back, null);
        if (length == 0)
            front = tmp;
        else
            back.next = tmp;
        back = tmp;
        length++;
    }

    // Insert new element before cursor.
    // Pre: length()>0, index()>=0
    void insertBefore(Object data) {
        if (length < 1)
            throw new RuntimeException(
                       "List Error: insertBefore() called on empty List");
        if (index < 0)
            throw new RuntimeException(
                       "List Error: insertBefore() called on undefined index");
        Node tmp = new Node(data, cursor.prev, cursor);
        if (index == 0)
            front = tmp;
        else
            cursor.prev.next = tmp;
        cursor.prev = tmp;
        //index++;
        length++;
    }

    // Inserts new element after cursor.
    // Pre: length()>0, index()>=0
    void insertAfter(Object data) {
        if (length < 1)
            throw new RuntimeException(
                       "List Error: insertAfter() called on empty List");
        if (index < 0)
            throw new RuntimeException(
                       "List Error: insertAfter() called on undefined index");
        Node tmp = new Node(data, cursor, cursor.next);
        if (index == length-1)
            back = tmp;
        else
            cursor.next.prev = tmp;
        cursor.next = tmp;
        length++;
    }

    // Deletes the front element. Pre: length()>0
    void deleteFront() {
        if (length < 1)
            throw new RuntimeException(
                       "List Error: deleteFront() called on empty List");
        front = front.next;
        front.prev = null;
        length--;
        if (index == 0) {
            cursor = null;
            index = -1;
        }
    }

    // Deletes the back element. Pre: length()>0
    void deleteBack() {
        if (length < 1)
            throw new RuntimeException(
                       "List Error: deleteBack() called on empty List");
        back = back.prev;
        back.next = null;
        length--;
        if (index == length-1) {
            cursor = null;
            index = -1;
        }
    }

    // Deletes the cursor element, making cursor undefined.
    // Pre: length()>0, index()>=0
    void delete() {
        if (length < 1)
            throw new RuntimeException("List Error: delete() called on empty List");
        if (index < 0)
            throw new RuntimeException(
                       "List Error: delete() called on undefined index");
        if (index == 0) {
            deleteFront();
        }else if (index == length - 1) {
            deleteBack();
        }else {
            cursor.prev.next = cursor.next;
            cursor.next.prev = cursor.prev;
            index = -1;
            length--;
        }
    }

    // Overrides Object's toString method. Returns a String
    // representation of this List consisting of a space
    // separated sequence of integers, with front on left.
    public String toString() {
        Node tmp = front;
        String str = "";
        while (tmp != null) {
            str = str + tmp.data + " ";
            tmp = tmp.next;
        }
        return str;
    }

    // Returns a new List representing the same integer sequence as this
    // List. The cursor in the new list is undefined, regardless of the
    // state of the cursor in this List. This List is unchanged.
    List copyList() {
        List copy = new List();
        Node tmp = front;
        while (tmp != null) {
            copy.append(tmp.data);
            tmp = tmp.next;
        }
        return copy;
    }

    // Returns a new List which is the concatenation of
    // this list followed by L. The cursor in the new List
    // is undefined. regardless of the states of the cursors
    // in this List and L. The states of this List and L are
    // unchanged.
    List concat(List L) {
        List con = copyList();
        Node tmp = L.front;
        while (tmp != null) {
            con.append(tmp.data);
            tmp = tmp.next;
        }
        return con;
    }
}
