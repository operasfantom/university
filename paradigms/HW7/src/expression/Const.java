package expression;

public strictfp class Const implements TripleExpression {
    private final Number value;

    public Const(Number value) {
        this.value = value;
    }

    @Override
    public int evaluate(int x, int y, int z) {
        return value.intValue();
    }
}
