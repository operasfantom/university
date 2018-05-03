"use strict";

var ZERO = new Const(0);
var ONE = new Const(1);

function Expression() {
}

Expression.prototype.evaluate = function () {
    var parameters = arguments;
    var args = [].slice.call(this.args);
    args = args.map(function (obj) {
        return obj.evaluate.apply(obj, parameters)
    });
    return this.operation.apply(null, args);
};

Expression.prototype.toString = function () {
    var args = [].slice.call(this.args);
    var result = args.map(function (obj) {
        return obj.toString();
    });
    return result.join(" ") + " " + this.symbol;
};

function Operation(operation, symbol, differentiation, simplification) {
    function Constructor() {
        this.args = arguments;
        this.operation = operation;
        this.symbol = symbol;
        this.diff = differentiation;
        this.simplify = simplification;
    }

    Object.setPrototypeOf(Constructor.prototype, Expression.prototype);
    Object.freeze(Constructor);
    return Constructor;
}

function areConstants() {
    return [].slice.call(arguments).every(function (x) {
        return x instanceof Const;
    })
}

function unionConstants(operation, a, b) {
    var F = factory(operation).apply(null, null);
    return new Const(F.operation(a.value, b.value));
}

function equalConstants(a, b) {
    if (a instanceof Const && b instanceof Const) {
        return a.value === b.value;
    }
    return false;
}

function equalClasses(a, b) {
    if (typeof a !== typeof b) {
        return false;
    }
    if (a instanceof Const) {
        return a.value === b.value;
    }
    if (a instanceof Variable) {
        return a.name === b.name;
    }
    if (a.args.length !== b.args.length) {
        return false;
    }
    for (var i = 0; i < a.args.length; ++i) {
        if (!equalClasses(a.args[i], b.args[i])) {
            return false;
        }
    }
    return true;
}

function Const(value) {
    this.value = value;

    Object.freeze(this);
}

Const.prototype.evaluate = function () {
    return this.value;
};

Const.prototype.toString = function () {
    return this.value.toPrecision();
};

Const.prototype.diff = function () {
    return ZERO;
};

Const.prototype.simplify = function () {
    return this;
};

function Variable(name) {
    this.name = name;
    Object.freeze(this);
}

Variable.prototype.evaluate = function (x, y, z) {
    if (this.name === "x") {
        return x;
    } else if (this.name === "y") {
        return y;
    } else if (this.name === "z") {
        return z;
    } else {
        throw Error(this.name + "is wrong name of Variable")
    }
};

Variable.prototype.toString = function () {
    return this.name;
};

Variable.prototype.diff = function (c) {
    if (this.name === c) {
        return ONE;
    } else {
        return ZERO;
    }
};

Variable.prototype.simplify = function () {
    return this;
};

var Add = Operation(
    function (a, b) {
        return a + b;
    },
    "+",
    function (c) {
        return new Add(this.args[0].diff(c), this.args[1].diff(c));
    },
    function () {
        var res0 = this.args[0].simplify(), res1 = this.args[1].simplify();

        if (areConstants(res0, res1)) return unionConstants(Add, res0, res1);

        if (equalConstants(res0, ZERO)) {
            return res1;
        }
        if (equalConstants(res1, ZERO)) {
            return res0;
        }
        return new Add(res0, res1);
    }
);

var Subtract = Operation(
    function (a, b) {
        return a - b;
    },
    "-",
    function (c) {
        return new Subtract(this.args[0].diff(c), this.args[1].diff(c));
    },
    function () {
        var res0 = this.args[0].simplify(), res1 = this.args[1].simplify();

        if (areConstants(res0, res1)) return unionConstants(Subtract, res0, res1);

        if (equalConstants(res1, ZERO)) {
            return res0;
        }
        if (equalConstants(res0, ZERO)) {
            if (res1 instanceof Const) {
                return new Const(-res1.value);
            } else {
                return new Negate(res1);
            }
        }
        return new Subtract(res0, res1);
    }
);

var Multiply = Operation(
    function (a, b) {
        return a * b;
    },
    "*",
    function (c) {
        return new Add(
            new Multiply(this.args[0].diff(c), this.args[1]),
            new Multiply(this.args[0], this.args[1].diff(c))
        );
    },
    function () {
        var res0 = this.args[0].simplify(), res1 = this.args[1].simplify();

        if (areConstants(res0, res1)) return unionConstants(Multiply, res0, res1);

        if (equalConstants(res0, ZERO) || equalConstants(res1, ZERO)) {
            return ZERO;
        }
        if (equalConstants(res0, ONE)) {
            return res1;
        }
        if (equalConstants(res1, ONE)) {
            return res0;
        }
        if (res1 instanceof Divide && areConstants(res1.args[0]) && res1.args[0].value === 1) { //(a * b) * (1 / c)
            return new Divide(res0, res1.args[1]);
        }
        if (res0 instanceof Divide && areConstants(res0.args[0]) && res0.args[0].value === 1) { //(1 / c) * (a * b)
            return new Divide(res1, res0.args[1]);
        }
        return new Multiply(res0, res1);
    }
);

var Divide = Operation(
    function (a, b) {
        return a / b;
    },
    "/",
    function (c) {
        return new Divide(
            new Subtract(
                new Multiply(this.args[0].diff(c), this.args[1]),
                new Multiply(this.args[0], this.args[1].diff(c))
            ),
            new Square(this.args[1])
        )
    },
    function () {
        var res0 = this.args[0].simplify(), res1 = this.args[1].simplify();

        if (areConstants(res0, res1)) return unionConstants(Divide, res0, res1);

        if (equalConstants(res0, res1)) {
            return ONE;
        }

        if (equalConstants(res0, ZERO)) {
            return ZERO;
        }
        if (res0 instanceof Divide && res1 instanceof Square) {
            if (equalClasses(res0.args[0], res1.args[0])) { //(a / b) / square(a)
                return new Divide(ONE, new Multiply(res0.args[1], res1.args[0]));
            }
        }
        if (res0 instanceof Divide) {
            if (res1 instanceof Multiply) {
                if (equalClasses(res0.args[0], res1.args[0])) { //(a / b) / (a * c)
                    return new Divide(ONE, new Multiply(res0.args[1], res1.args[1]));
                }
                if (equalClasses(res0.args[0], res1.args[1])) { //(a / b) / (c * a)
                    return new Divide(ONE, new Multiply(res0.args[1], res1.args[0]));
                }
            }
        }
        if (res0 instanceof Multiply) {
            if (equalClasses(res0.args[0], res1)) { //(a * b) / a
                return res0.args[1];
            }
            if (equalClasses(res0.args[1], res1)) { //(a * b) / b
                return res0.args[0];
            }
            if (res1 instanceof Square) {
                if (equalClasses(res0.args[0], res1.args[0])) { //(a * b) / a^2
                    return new Divide(res0.args[1], res1.args[0]);
                }
                if (equalClasses(res0.args[1], res1.args[0])) { //(a * b) / b^2
                    return new Divide(res0.args[0], res1.args[0]);
                }
            }
            if (res1 instanceof Multiply){
                for (var i = 0; i < 2; ++i){
                    for (var j = 0; j < 2; ++j){
                        if (equalClasses(res0.args[i], res1.args[j])){
                            return new Divide(res0.args[i ^ 1], res1.args[j ^ 1]).simplify();
                        }
                    }
                }
            }
        }
        if (res0 instanceof Negate){
            return new Negate(new Divide(res0.args[0], res1).simplify());
        }
        if (res1 instanceof Negate){
            return new Negate(new Divide(res0, res1.args[0]).simplify());
        }
        return new Divide(res0, res1);
    }
);

var Negate = Operation(
    function (a) {
        return -a;
    },
    "negate",
    function (c) {
        return new Negate(this.args[0].diff(c));
    },
    function () {
        var res = this.args[0].simplify();
        if (areConstants(res)) {
            return new Const(-res.value);
        }
        if (res instanceof Subtract) {
            return new Subtract(res.args[1], res.args[0]).simplify();
        }
        if (res instanceof Divide) {
            if (areConstants(res.args[0])) {
                return new Divide(new Negate(res.args[0]), res.args[1]).simplify();
            }
            if (areConstants(res.args[1])) {
                return new Divide(res.args[0], new Negate(res.args[1])).simplify();
            }
        }
        if (res instanceof Multiply) {
            if (areConstants(res.args[0])) {
                return new Multiply(new Negate(res.args[0]), res.args[1]).simplify();
            }
            if (areConstants(res.args[1])) {
                return new Multiply(res.args[0], new Negate(res.args[1])).simplify();
            }
        }
        return new Negate(res);
    }
);

var Square = Operation(
    function (a) {
        return a * a;
    },
    "square",
    function (c) {
        return new Multiply(
            new Multiply(
                new Const(2),
                this.args[0]
            ),
            this.args[0].diff(c)
        )
    },
    function () {
        var res0 = this.args[0].simplify();
        if (areConstants(res0)) {
            return new Const(Math.pow(res0.value, 2));
        }
        if (res0.toString().length <= 4) {
            return new Multiply(res0, res0).simplify();
        }
        if (res0 instanceof Negate){
            return res0.args[0].simplify()
        }
        return new Square(res0);
    }
);

var Abs = Operation(
    function (a) {
        return Math.abs(a);
    },
    "abs",
    function (c) {
        return new Multiply(
            this.args[0].diff(c),
            new Divide(
                this.args[0],
                new Square(new Sqrt(this.args[0]))
            )
        )
    }
);

var Sqrt = Operation(
    function (a) {
        return Math.sqrt(Math.abs(a));
    },
    "sqrt",
    function (c) {
        return new Divide(
            (new Abs(this.args[0])).diff(c),
            new Multiply(
                new Const(2),
                this
            )
        )
    },
    function () {
        var res0 = this.args[0].simplify();
        if (areConstants(res0)) {
            return unionConstants(Sqrt, res0);
        }
        return new Sqrt(res0);
    }
);

var Power = Operation(
    function (a, b) {
        return Math.pow(a, b);
    },
    "pow",
    function (c) {
        return new Add(
            new Multiply(
                this.args[1],
                new Multiply(
                    new Power(
                        this.args[0],
                        new Subtract(
                            this.args[1],
                            ONE
                        )
                    ),
                    this.args[0].diff(c)
                )
            ),

            new Multiply(
                this,
                new Multiply(
                    this.args[1].diff(c),
                    new Log(
                        new Const(Math.E),
                        this.args[0]
                    )
                )
            )
        )
    },
    function () {
        var res0 = this.args[0].simplify(), res1 = this.args[1].simplify();
        if (areConstants(res0, res1)) {
            return unionConstants(Power, res0, res1);
        }
        return new Power(res0, res1);
    }
);

var Ln = Operation(
    function (a) {
        return Math.log(Math.abs(a));
    },
    "ln",
    function (c) {
        return new Divide(
            this.args[0].diff(c),
            this.args[0]
        )
    }
);

var Log = Operation(
    function (a, b) {
        return Math.log(Math.abs(b)) / Math.log(Math.abs(a));
    },
    "log",
    function (c) {
        /*return new Divide(
            new Ln(this.args[1]),
            new Ln(this.args[0])
        ).diff(c)*/
        var argln = [
            new Ln(this.args[1]),
            new Ln(this.args[0])
        ];
        var arglog2 = [
            new Log(new Const(2), this.args[1]),
            new Log(new Const(2), this.args[0])
        ];
        return new Divide(
            new Subtract(
                new Multiply(argln[0].diff(c), arglog2[1]),
                new Multiply(arglog2[0], argln[1].diff(c))
            ),
            new Multiply(arglog2[1], new Log(new Const(Math.E), this.args[0]))
        )
    },
    function () {
        var res0 = this.args[0].simplify(), res1 = this.args[1].simplify();
        if (areConstants(res0, res1)) {
            return unionConstants(Log, res0, res1);
        }
        return new Log(res0, res1);
    }
);

function factory(operation) {
    function F(args) {
        operation.apply(this, args);
    }

    F.prototype = operation.prototype;

    return function () {
        return new F(arguments);
    }
}

function parse(s) {
    var arr = s.split(/\s+/);

    function step(stack, token) {

        function apply(f, n) {
            var args = stack.splice(-n, n);
            stack.push(factory(f).apply(null, args));
        }

        switch (token) {
            case "":
                break;
            case "+":
                apply(Add, 2);
                break;
            case "-":
                apply(Subtract, 2);
                break;
            case "*":
                apply(Multiply, 2);
                break;
            case "/":
                apply(Divide, 2);
                break;
            case "negate":
                apply(Negate, 1);
                break;
            case "square":
                apply(Square, 1);
                break;
            case "sqrt":
                apply(Sqrt, 1);
                break;
            case "pow":
                apply(Power, 2);
                break;
            case "log":
                apply(Log, 2);
                break;
            default:
                if (isNaN(token)) {
                    stack.push(new Variable(token));
                } else {
                    stack.push(new Const(parseInt(token)));
                }
        }
        return stack;
    }

    return arr.reduce(step, [])[0]
}

function test() {
    var expr = parse('2 4 x z - * + 1 2 y z - * + log').diff('y').simplify();
    // var expr = parse('x y log').diff('x');
    console.log(expr.toString());
    expr = expr.simplify();
    console.log(expr.toString());
    console.log(expr.evaluate(2, 3, 2))
}

// test();