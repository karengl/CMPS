// $Id: treemap.java,v 1.14 2016-02-15 19:58:09-08 - - $
// Karen Lee kglee

import static java.lang.System.*;

class treemap {

   class tree {
      String key;
      String value;
      tree left;
      tree right;
   }
   tree root = null;

   public String get (String key) {
      tree curr = root;
      while (curr != null) {
         int cmp = curr.key.compareTo(key);
         if (cmp == 0) return curr.value;
         if (cmp < 0) curr = curr.right;
                 else curr = curr.left;
      }
      return null;
   }

   public String put (String key, String value) {
      boolean left = false;
      if (root == null) {
         tree node = new tree();
         node.key = key;
         node.value = value;
         root = node;
         return root.key;
      }
      tree curr = root;
      tree prev = root;
      String val = "";
      while (curr != null) {
         int cmp = curr.key.compareTo(key);
         if (cmp == 0) {
            val = curr.value;
            curr.value = value;
            return val;
         }
         if (cmp < 0) {
            if (curr.right == null) prev = curr;
            curr = curr.right;
            left = false;
         }else {
            if (curr.left == null) prev = curr;
            curr = curr.left;
            left = true;
         }
      }
      tree node = new tree();
      node.key = key;
      node.value = value;
      if (left) prev.left = node;
      else prev.right = node;
      return node.key;
   }

   public void debug_tree () {
      debug_tree_recur (root, 0);
   }

   private void debug_tree_recur (tree node, int depth) {
      if (node == null) return;
      debug_tree_recur(node.left, depth + 1);
      out.printf("%3d \"%s\" \"%s\" %s %s%n", depth, node.key,
                             node.value, node.left, node.right);
      debug_tree_recur(node.right, depth + 1);
   }

}
