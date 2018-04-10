package expression;

public class Unary implements CommonExpression {
    private CommonExpression function;

    public Unary(CommonExpression function) {
        this.function = function;
    }

    @Override
    public double evaluate(double x) {
        return 0;
    }

    @Override
    public int evaluate(int x) {
        return -(function.evaluate(x));
    }

    @Override
    public int evaluate(int x, int y, int z) {
        return -(function.evaluate(x, y, z));
    }
}
