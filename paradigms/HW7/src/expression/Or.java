package expression;

public strictfp class Or extends AbstractOperation {
    public Or(TripleExpression x, TripleExpression y) {
        super(x, y);
    }

    @Override
    protected int calculate(int x0, int x1) {
        return x0 | x1;
    }

}
