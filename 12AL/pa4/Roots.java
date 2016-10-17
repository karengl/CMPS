// Roots.java
// Karen Lee
// kglee
// pa4
// Finds the real roots of a polynomial function

import java.util.Scanner;

public class Roots {
    public static void main(String[] args) {
        int degree;
        double root, left, right, a, b;
        boolean found = false;
        double tolerance = Math.pow(10, -7);
        double resolution = Math.pow(10, -2);
        double threshold = Math.pow(10, -3);
        double[] C;

        Scanner console = new Scanner(System.in);
        System.out.print("Enter the degree: ");
        degree = console.nextInt();
        C = new double[degree + 1];

        System.out.print("Enter " + (degree + 1) + " coefficients: ");
        for (int i = 0; i < C.length; i++)
            C[i] = console.nextDouble();

        System.out.print("Enter the left and right endpoints: ");
        left = console.nextDouble();
        right = console.nextDouble();
        System.out.println();

        a = left;
        b = a + resolution;
        double[] D = diff(C);
        // Tests for the sign change on the interval [a, b] with the width of resolution
        while (b < right) {
            if (poly(C, a) * poly(C, b) < 0) {
                root = findRoot(C, a, b, tolerance);
                if (Math.abs(poly(C, root)) < threshold) {
                    found = true;
                    System.out.printf("Root found at %.5f%n", root);
                }
            }else if (poly(D, a) * poly(D, b) < 0) {
                root = findRoot(D, a, b, tolerance);
                if (Math.abs(poly(C, root)) < threshold) {
                    found = true;
                    System.out.printf("Root found at %.5f%n", root);
                }
            }
            // Increment so the subinterval is not the exact same as the previous one
            a += resolution + (resolution / 1000);
            b += resolution + (resolution / 1000);
        }
        if (!found)
            System.out.println("No roots were found in the specified range.");
    }
        
    // Returns the sum of the polynomial C
    static double poly(double[] C, double x) {
        double sum = 0.0;
        
        for (int i = C.length - 1; i >= 0; i--)
            sum += C[i] * Math.pow(x, i);
        
        return sum;
    }

    // Returns an array of coefficients for the derivative of poly C
    static double[] diff(double[] C) {
        double[] D = new double[C.length - 1];
        int power = C.length - 1;

        for (int i = D.length - 1; i >= 0; i--) {
            D[i] = power * C[i + 1];
            power--;
        }
        return D;
    }

    // Finds the roots of the polynomial C
    static double findRoot(double[] C, double a, double b, double tolerance) {
        double width = b-a, mid = a;

        while (width > tolerance) {
            mid = (a + b) / 2;
            if (poly(C, a) * poly(C, mid) < 0)
                b = mid;
            else
                a = mid;
            width = b-a;
        }
        return mid;
    }
}
