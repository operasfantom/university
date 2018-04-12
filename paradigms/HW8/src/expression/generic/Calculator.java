package expression.generic;

import expression.TripleExpression;
import expression.exceptions.CheckedParserException;
import expression.parser.ExpressionParser;
import numbers.AbstractNumber;

import java.util.function.Function;

public class Calculator<T extends AbstractNumber<T>, S> {
    private ExpressionParser parser = new ExpressionParser();
    private Function<String, T> constructor;
    private Function<T, S> getter;
    private TripleExpression<T> tripleExpression;

    Calculator(Function<String, T> constructor, Function<T, S> getter, String expression) throws CheckedParserException {
        this.constructor = constructor;
        this.getter = getter;
        this.tripleExpression = parser.parse(expression, constructor.apply("0"));
    }

    Object[][][] calculate(int x1, int x2, int y1, int y2, int z1, int z2) {
        Object[][][] result = new Object[x2 - x1 + 1][y2 - y1 + 1][z2 - z1 + 1];
        for (int i = x1; i <= x2; i++) {
            for (int j = y1; j <= y2; j++) {
                for (int k = z1; k <= z2; k++) {
                    try {
                        result[i - x1][j - y1][k - z1] = getter.apply(tripleExpression.evaluate(constructor.apply(Integer.toString(i)), constructor.apply(Integer.toString(j)), constructor.apply(Integer.toString(k))));
                    } catch (Exception e) {
                        result[i - x1][j - y1][k - z1] = null;
                    }
                }
            }
        }
        return result;
    }
}
