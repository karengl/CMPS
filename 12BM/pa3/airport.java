// $Id: airport.java,v 1.38 2016-02-15 19:52:42-08 - - $
// Karen Lee kglee

import java.io.*;
import java.util.Scanner;
import static java.lang.System.*;

class airport {
   private static final String STDIN_FILENAME = "-";

   public static treemap load_database (String database_name) {
      treemap tree = new treemap ();
      try {
         Scanner database = new Scanner (new File (database_name));
         for (int linenr = 1; database.hasNextLine(); ++linenr) {
            String line = database.nextLine();
            if (line.matches ("^\\s*(#.*)?$")) continue;
            String[] keyvalue = line.split (":");
            if (keyvalue.length != 2) {
               misc.warn (database_name, linenr, "invalid line");
               continue;
            }
            tree.put (keyvalue[0], keyvalue[1]);
         }
         database.close();
      }catch (IOException error) {
         misc.warn (error.getMessage());
         err.printf("Usage: airport [-d] database%n");
      }
      return tree;
   } 

   public static void main (String[] args) {
      boolean option = false;
      if (args.length == 0) {
         misc.warn("Missing the airport database");
         err.printf("Usage: airport [-d] database%n");
      }else {
         for (int argix = 0; argix < args.length; ++argix) {
            // Check for option [-d]
            if (argix == 0 && args[0].matches("-\\w")) {
               if (args[0].substring(1).equals("d")) {
                  option = true;
               }else {
                  misc.warn("Not a valid option");
                  err.printf("Usage: airport [-d] database%n");
                  exit(misc.exit_status);
               }
            }else {
               treemap tree = load_database (args[argix]);
               if (option) {
                  tree.debug_tree();
                  break;
               }
               Scanner stdin = new Scanner (in);
               while (stdin.hasNextLine()) {
                  String airport = stdin.nextLine();
                  airport = airport.toUpperCase().trim();
                  String[] count_words = airport.split(" +");
                  // Ignores lines starting with #
                  if (airport.substring(0, 1).equals("#")) continue;
                  // Prints an error when lines have 
                  // more than one airport code
                  if (count_words.length > 1) {
                     err.printf
                          ("invalid line%n");
                     continue;
                  }
                  String airport_name = tree.get (airport);
                  if (airport_name == null)
                     out.printf("%s: no such airport%n", airport);
                  else
                     out.printf("%s = %s%n", airport, airport_name);
               }
            }
         }
      }
      exit (misc.exit_status);
   }
}
