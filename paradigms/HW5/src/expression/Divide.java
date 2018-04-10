package expression;

public strictfp class Divide extends AbstractOperation {

    public Divide(ImplicitExpression x, ImplicitExpression y) {
        super(x, y);
    }

    @Override
    protected int calculate(int x, int y) {
        return x / y;
    }

    @Override
    protected double calculate(double x, double y) {
        return x / y;
    }
}
