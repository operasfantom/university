package expression;

public strictfp class And extends AbstractOperation {
    public And(TripleExpression x, TripleExpression y) {
        super(x, y);
    }

    @Override
    protected int calculate(int x0, int x1) {
        return x0 & x1;
    }
}
