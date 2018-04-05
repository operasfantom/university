package expression;

public strictfp class Xor extends AbstractOperation {
    public Xor(TripleExpression x, TripleExpression y) {
        super(x, y);
    }

    @Override
    protected int calculate(int x0, int x1) {
        return x0 ^ x1;
    }

}
