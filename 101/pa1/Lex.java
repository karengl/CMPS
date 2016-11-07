// Karen Lee
// kglee
// pa1
// Lex.java

import java.io.*;
import java.util.Scanner;

public class Lex {
    public static void main(String[] args) throws IOException{
        int j = 0;
        int len = 0;
        int cmp = 0;
        if (args.length != 2) {
            System.err.printf("Usage: Lex infile outfile%n");
            System.exit(1);
        }
        Scanner console = new Scanner(new File(args[0]));
        PrintWriter out = new PrintWriter(new FileWriter(args[1]));
        // Count amount of lines in file and then close
        while (console.hasNextLine()) {
            len++;
            console.nextLine();
        }
        console.close();
        // Reopen file to place lines into an array
        String list[] = new String[len];
        console = new Scanner(new File(args[0]));
        for (int i = 0; i < len; i++) {
            list[i] = console.nextLine();
        }

        List L = new List();
        L.prepend(0);
        for (int i = 1; i < list.length; i++) {
            j = i - 1;
            L.moveBack();
            while (j > -1) {
                cmp = list[i].compareTo(list[L.get()]);
                if (cmp > 0)
                    break;
                j--;
                L.movePrev();
            }
            if (L.index() > -1)
                L.insertAfter(i);
            else
                L.prepend(i);
        }

        L.moveFront();
        while (L.index() > -1) {
            out.println(list[L.get()]);
            L.moveNext();
        }
        console.close();
        out.close();
    }
}
