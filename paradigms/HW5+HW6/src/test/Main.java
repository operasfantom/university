package test;

import expression.CommonExpression;
import expression.parser.ExpressionParser;

public class Main {
    private static double f(double x) {
        return x * (x - 2) * x + 1;
    }

    public static void main(String[] args) {
        //String formula = "-(-(-\t\t-5 + 16   *x*y) + 1 * z) -(((-11)))";
        String formula = "count 5";
        ExpressionParser ep = new ExpressionParser();
        CommonExpression expression = ep.parse(formula);
        System.out.println(expression.evaluate(0, 0, 0));

    }
}
