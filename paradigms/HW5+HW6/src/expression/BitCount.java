package expression;

public class BitCount implements CommonExpression {
    private CommonExpression function;

    public BitCount(CommonExpression function) {
        this.function = function;
    }

    @Override
    public double evaluate(double x) {
        return 0;
    }

    @Override
    public int evaluate(int x) {
        return Integer.bitCount(function.evaluate(x));
    }

    @Override
    public int evaluate(int x, int y, int z) {
        return Integer.bitCount(function.evaluate(x, y, z));
    }
}
