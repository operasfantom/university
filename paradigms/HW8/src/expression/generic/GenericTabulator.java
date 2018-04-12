package expression.generic;

import numbers.*;

public class GenericTabulator implements Tabulator {

    @Override
    public Object[][][] tabulate(String mode, String expression, int x1, int x2, int y1, int y2, int z1, int z2) throws Exception {
        Calculator calculator;
        switch (mode) {
            case "i":
                calculator = new Calculator<>(CheckedIntegerNumber::new, CheckedIntegerNumber::getValue, expression);
                break;
            case "d":
                calculator = new Calculator<>(DoubleNumber::new, DoubleNumber::getValue, expression);
                break;
            case "bi":
                calculator = new Calculator<>(BigIntegerNumber::new, BigIntegerNumber::getValue, expression);
                break;
            case "u":
                calculator = new Calculator<>(IntegerNumber::new, IntegerNumber::getValue, expression);
                break;
            case "l":
                calculator = new Calculator<>(LongNumber::new, LongNumber::getValue, expression);
                break;
            case "s":
                calculator = new Calculator<>(ShortNumber::new, ShortNumber::getValue, expression);
                break;
            default:
                throw new Exception("unknown mode: " + mode);
        }
        return calculator.calculate(x1, x2, y1, y2, z1, z2);
    }


}
