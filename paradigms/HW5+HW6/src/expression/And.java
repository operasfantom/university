package expression;

public strictfp class And extends AbstractOperation {
    public And(CommonExpression x, CommonExpression y) {
        super(x, y);
    }

    @Override
    protected int calculate(int x, int y) {
        return x & y;
    }

    @Override
    protected double calculate(double x, double y) {
        return 0;
    }
}
