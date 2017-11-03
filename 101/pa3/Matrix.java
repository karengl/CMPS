// Karen Lee
// kglee
// pa3
// Matrix.java

class Matrix {
    private class Entry {
        int col;
        double val;

        Entry(int col, double val) {
            this.col = col;
            this.val = val;
        }

        public String toString() {
            return "(" + col + ", " + val + ")";
        }

        public boolean equals(Object x) {
            Entry E;
            boolean eq = false;
            if (x instanceof Entry) {
                E = (Entry) x;
                eq = (this.col == E.col && this.val == E.val);
            }
            return eq;
        }
    }

    List[] row;

    // Makes a new n x n zero Matrix. pre: n>=1
    Matrix(int n) {
        if (n < 1)
            throw new RuntimeException("Matrix Error: Matrix called with non-positive column");
        row = new List[n + 1];
        for (int i = 1; i < n + 1; i++)
            row[i] = new List();
    }

    // Returns n, the number of rows and columns of this Matrix
    int getSize() {
        return row.length - 1;
    }

    // Returns the number of non-zero entries in this Matrix
    int getNNZ() {
        int count = 0;
        for (int i = 1; i <= getSize(); i++)
            count += row[i].length();
        return count;
    }

    // overrides Object's equals() method
    public boolean equals(Object x) {
        Matrix M;
        if (x instanceof Matrix) {
            M = (Matrix) x;
            if (getSize() != M.getSize())
                return false;
            for (int i = 1; i <= getSize(); i++) {
                if (!(row[i].equals(M.row[i])))
                    return false;
            }
        } else {
            return false;
        }
        return true;
    }

    // sets this Matrix to the zero state
    void makeZero() {
        for (int i = 1; i <= getSize(); i++)
            row[i] = new List();
    }

    // returns a new Matrix having the same entries as this Matrix
    Matrix copy() {
        Matrix M = new Matrix(getSize());
        for (int i = 1; i <= getSize(); i++) {
            row[i].moveFront();
            while (row[i].index() > -1) {
                Entry tmp = (Entry) row[i].get();
                M.changeEntry(i, tmp.col, tmp.val);
                row[i].moveNext();
            }
        }
        return M;
    }

    // changes ith row, jth column of this Matrix to x
    // pre: 1<=i<=getSize(), 1<=j<=getSize()
    void changeEntry(int i, int j, double x) {
        if (i < 1 || i > getSize())
            throw new RuntimeException("Matrix Error: changeEntry() called on invalid row");
        if (j < 1 || j > getSize())
            throw new RuntimeException("Matrix Error: changeEntry() called on invalid column");

        row[i].moveFront();
        boolean found = false;
        while (row[i].index() > -1) {
            Entry tmp = (Entry) row[i].get();
            if (tmp.col == j) found = true;
            if (found) {
                if (x == 0.0) {      // If x==0 and A[i][j]!=0
                    row[i].delete();
                    return;
                } else {             // If x!=0 and A[i][j]!=0
                    tmp.val = x;
                    return;
                }
            }
            row[i].moveNext();
        }
        if (!found && x != 0.0) {    // If x!=0 and A[i][j]==0
            row[i].moveFront();
            if (row[i].index() == -1) {  // If List is empty
                row[i].append(new Entry(j, x));
            }else {
                // Get position to insert either by falling off list
                // or going past col j
                while (row[i].index() != -1 && ((Entry) row[i].get()).col < j) {
                    row[i].moveNext();
                }
                if (row[i].index() == -1)    // Cursor fell off list
                    row[i].append(new Entry(j, x));
                else                         // Insert after col j
                    row[i].insertBefore(new Entry(j, x));
            }
        }
    }

    // returns a new Matrix that is the sum of this Matrix with M
    Matrix scalarMult(double x) {
        Matrix M = this.copy();
        if (x == 0.0) {
            M.makeZero();
            return M;
        }
        for (int i = 1; i <= getSize(); i++) {
            M.row[i].moveFront();
            while (M.row[i].index() > -1) {
                Entry tmp = (Entry) M.row[i].get();
                M.changeEntry(i, tmp.col, tmp.val * x);
                M.row[i].moveNext();
            }
        }
        return M;
    }

    // returns a new Matrix that is the sum of this Matrix with M
    // pre: getSize()==M.getSize()
    Matrix add(Matrix M) {
        if (getSize() != M.getSize())
            throw new RuntimeException("Matrix error: add() called on Matrices of different sizes");
        if (this == M)
            return this.copy().scalarMult(2);
        Matrix A = new Matrix(getSize());
        for (int i = 1; i <= getSize(); i++)
            A.row[i] = addsub(row[i], M.row[i], true);
        return A;
    }

    // returns a new Matrix that is the difference of this Matrix with M
    // pre: getSize()==M.getSize()
    Matrix sub(Matrix M) {
        if (getSize() != M.getSize())
            throw new RuntimeException("Matrix error: sub() called on Matrices of different sizes");
        if (this == M)
            return new Matrix(getSize());
        Matrix A = new Matrix(getSize());
        for (int i = 1; i <= getSize(); i++)
            A.row[i] = addsub(row[i], M.row[i], false);
        return A;
    }

    // returns a new Matrix that is the transpose of this Matrix
    Matrix transpose() {
        Matrix M = new Matrix(getSize());
        for (int i = 1; i <= getSize(); i++) {
            row[i].moveFront();
            while (row[i].index() > -1) {
                Entry tmp = (Entry) row[i].get();
                M.changeEntry(tmp.col, i, tmp.val);
                row[i].moveNext();
            }
        }
        return M;
    }

    // returns a new Matrix that is the product of this Matrix with M
    // pre: getSize==M.getSize()
    Matrix mult(Matrix M) {
        if (getSize() != M.getSize())
            throw new RuntimeException("Matrix error: mult() called on Matrices of different sizes");
        Matrix A = new Matrix(getSize());
        Matrix B = M.transpose();
        for (int i = 1; i <= getSize(); i++) {
            if (row[i].length() == 0) continue;
            for (int j = 1; j <= getSize(); j++) {
                if (B.row[j].length() == 0) continue;
                A.changeEntry(i, j, dot(row[i], B.row[j]));
            }
        }
        return A;
    }

    // overrides Object's toString() method
    public String toString() {
        String str = "";
        for (int i = 1; i <= getSize(); i++) {
            if (row[i].length() > 0)
                str += i + ": " + row[i] + "\n";
        }
        return str;
    }

    // helper for adding and subtracting two Lists
    private List addsub(List P, List Q, boolean adds) {
        List L = new List();
        P.moveFront();
        Q.moveFront();
        while (P.index() > -1 || Q.index() > -1) {
            // Both cursors in Lists P and Q are defined
            if (P.index() > -1 && Q.index() > -1) {
                Entry p = (Entry) P.get();
                Entry q = (Entry) Q.get();
                if (p.col == q.col) {
                    if (adds && (p.val + q.val != 0))
                        L.append(new Entry(p.col, p.val + q.val));
                    else if (!adds && (p.val - q.val != 0))
                        L.append(new Entry(p.col, p.val - q.val));
                    P.moveNext();
                    Q.moveNext();
                }else if (p.col < q.col) {
                    L.append(new Entry(p.col, p.val));
                    P.moveNext();
                }else if (p.col > q.col) {
                    if (adds)
                        L.append(new Entry(q.col, q.val));
                    else
                        L.append(new Entry(q.col, -1.0 * q.val));
                    Q.moveNext();
                }
            }else if (P.index() == -1) {     // If cursor in P is undefined
                Entry q = (Entry) Q.get();
                if (adds)
                    L.append(new Entry(q.col, q.val));
                else
                    L.append(new Entry(q.col, -1.0 * q.val));
                Q.moveNext();
            }else {          // If cursor in Q is undefined
                Entry p = (Entry) P.get();
                L.append(new Entry(p.col, p.val));
                P.moveNext();
            }
        }
        return L;
    }

    // helper for dot product of two Lists
    private static double dot(List P, List Q) {
        double prd = 0.0;
        P.moveFront();
        Q.moveFront();
        while (P.index() > -1 && Q.index() > -1) {
            Entry p = (Entry) P.get();
            Entry q = (Entry) Q.get();
            if (p.col == q.col) {
                prd += p.val * q.val;
                P.moveNext();
                Q.moveNext();
            }else if (p.col < q.col) {   // 0's don't add to dot product
                P.moveNext();
            }else {
                Q.moveNext();
            }
        }
        return prd;
    }
}
