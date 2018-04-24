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
    }
});