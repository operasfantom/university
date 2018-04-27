// import lib from "../objectExpression.js";

TestCase("UnitTest", {
    "test const": function() {
        var expr = new Const(4);
        assertEquals(4, expr.evaluate(100));
    },
    "test variable": function() {
        var expr = new Variable("x");
        assertEquals(-1, expr.evaluate(-1));
    },
    "test negate": function () {
      var expr = new Negate(new Variable('x'));
      assertEquals(-2, expr.evaluate(2))
    },
    "test negate hard": function () {
        var expr = new Divide(new Negate(new Variable('x')), new Const(2));
        assertEquals(-5, expr.evaluate(10))
    },
    "test negate toString()": function () {
        var expr = new Negate(new Variable("x"));
        assertEquals("x negate", expr.toString());
        assertEquals(5, expr.evaluate(-5));
    },
    "test sample" : function () {
        var expr = new Subtract(
            new Multiply(
                new Const(2),
                new Variable("x")
            ),
            new Const(3)
        );
        assertEquals(7, expr.evaluate(5));
    },
    "test parse": function () {
        var expr = parse('x 2 +');
        assertEquals(7, console.log(expr.evaluate(5)));
    },
    "test diff": function () {
        var expr = new Add(new Variable('x'), new Const(2)).diff('x');
        assertEquals(1, expr.evaluate(5));
    },
    "test modify easy": function () {
        var expr = new Add(new Variable('x'), new Sqrt(new Subtract(new Variable('y'), new Divide(new Square(new Const(3)), new Const(4)))));
        assertEquals(1.5, expr.evaluate(0, 0, 0))
    },
    "test modify 2": function () {
        var expr = new Sqrt(new Subtract(new Variable('y'), new Divide(new Square(new Const(3)), new Const(4)))).diff('y')
        assertEquals(-1.0, expr.evaluate(2.00000000000000000000,2.00000000000000000000,2.00000000000000000000));
    },
    "test random": function () {
        var expr = new Log(new Power(new Subtract(new Subtract(new Const(-1297918201), new Variable('y')), new Variable('x')), new Subtract(new Power(new Const(-1121176728), new Const(890377089)), new Multiply(new Variable('x'), new Const(-1429575531)))), new Negate(new Power(new Variable('x'), new Const(-1595116792))))
    },
    "test power diff": function () {
        var expr = new Power(new Variable('x'), new Variable('y')).diff('x');
        assertEquals(12, expr.evaluate(2, 3, 2));
    },
    "test log diff": function () {
        var expr = new Log(new Variable('x'), new Variable('y')).diff('x');
        assertEquals(-1 / (2 * Math.log(2)), expr.evaluate(2, 2, 2));
    },
    "test log diff 2": function () {
        var expr = new Log(new Add(new Const(2), new Multiply(new Const(4), new Subtract(new Variable('x'), new Variable('z')))), new Const(1)).diff('x');
        assertEquals(0, expr.evaluate(2, 2, 3));
    },
    "test simplify": function () {
        var expr = new Variable(0).simplify();
        console.log(expr.toString());
    }
});