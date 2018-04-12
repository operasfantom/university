package expression;

import expression.generic.GenericTabulator;

public class Main {
    public static void main(String[] args) throws Exception {
        GenericTabulator tabulator = new GenericTabulator();
        Object[][][] result = tabulator.tabulate("s", "10", 2147483629, 2147483629, 2147483628, 2147483628, 2147483630, 2147483630);
        System.out.println(result[0][0][0]);
    }
}
