package expression;

public strictfp class Add extends AbstractOperation {
    public Add(ImplicitExpression x, ImplicitExpression y) {
        super(x, y);
    }

    @Override
    protected int calculate(int x, int y) {
        return x + y;
    }

    @Override
    protected double calculate(double x, double y) {
        return x + y;
    }
}
