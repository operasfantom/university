package search;

public class BinarySearchSpan {
    public static void main(String[] args) {
        if (args.length == 0) {
            System.err.println("Incorrect input");
            return;
        }

        int n = args.length - 1, x;
        int[] array = new int[n];
        try {
            for (int i = 0; i < n; i++) {
                array[i] = Integer.parseInt(args[i + 1]);
            }
            x = Integer.parseInt(args[0]);
            int rightBound = IterativeSearch.search(array, x);//first less
            int leftBound = RecursiveSearch.search(array, x);//first less or equal
            System.out.println(leftBound + " " + (rightBound - leftBound));

        } catch (NumberFormatException e) {
            System.err.println("Couldn't parse arguments");
        }
    }
}

