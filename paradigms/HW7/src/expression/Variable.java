package expression;

public strictfp class Variable implements TripleExpression {
    public final String name;

    public Variable(String name) {
        this.name = name;
    }

    @Override
    public int evaluate(int x, int y, int z) {
        if ("x".equals(this.name)) {
            return x;
        } else if ("y".equals(this.name)) {
            return y;
        } else if ("z".equals(this.name)) {
            return z;
        } else {
            return 0;
        }
    }
}
