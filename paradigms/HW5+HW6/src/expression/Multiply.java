package expression;

public strictfp class Multiply extends AbstractOperation {
    public Multiply(CommonExpression x, CommonExpression y) {
        super(x, y);
    }

    @Override
    protected int calculate(int x, int y) {
        return x * y;
    }

    @Override
    protected double calculate(double x, double y) {
        return x * y;
    }
}
