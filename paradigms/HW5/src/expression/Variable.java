package expression;

public strictfp class Variable implements ImplicitExpression {
    public final String name;

    public Variable(String name) {
        this.name = name;
    }

    @Override
    public int evaluate(int x) {
        return x;
    }

    @Override
    public double evaluate(double x) {
        return x;
    }

    @Override
    public int evaluate(int x, int y, int z) {
        if ("x".equals(this.name)) {
            return x;
        }
        if ("y".equals(this.name)) {
            return y;
        }
        if ("z".equals(this.name)) {
            return z;
        }
        return 0;
    }
}
