package expression;

public strictfp abstract class AbstractOperation implements CommonExpression {
    private CommonExpression operand0;
    private CommonExpression operand1;

    public AbstractOperation(CommonExpression x, CommonExpression y) {
        operand0 = x;
        operand1 = y;
    }

    protected abstract int calculate(int x, int y);

    protected abstract double calculate(double x, double y);

    public int evaluate(int x) {
        int x0 = operand0.evaluate(x);
        int x1 = operand1.evaluate(x);
        return calculate(x0, x1);
    }

    public double evaluate(double x) {
        double x0 = operand0.evaluate(x);
        double x1 = operand1.evaluate(x);
        return calculate(x0, x1);
    }

    @Override
    public int evaluate(int x, int y, int z) {
        int x0 = operand0.evaluate(x, y, z);
        int x1 = operand1.evaluate(x, y, z);
        return calculate(x0, x1);
    }
}
