package expression;

import java.util.ArrayList;
import java.util.List;

public strictfp abstract class AbstractOperation implements ImplicitExpression {
    private List<ImplicitExpression> operands = new ArrayList<>();

    public AbstractOperation(ImplicitExpression x, ImplicitExpression y) {
        operands.add(x);
        operands.add(y);
    }

    protected abstract int calculate(int x, int y);

    protected abstract double calculate(double x, double y);


    public int evaluate(int x) {
        int x0 = operands.get(0).evaluate(x);
        int x1 = operands.get(1).evaluate(x);
        return calculate(x0, x1);
    }

    public double evaluate(double x) {
        double x0 = operands.get(0).evaluate(x);
        double x1 = operands.get(1).evaluate(x);
        return calculate(x0, x1);
    }

    @Override
    public int evaluate(int x, int y, int z) {
        int x0 = operands.get(0).evaluate(x, y, z);
        int x1 = operands.get(1).evaluate(x, y, z);
        return calculate(x0, x1);
    }
}
