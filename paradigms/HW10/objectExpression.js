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
    var F = creator(operation).apply(null, null);
    return new Const(F.operation(a.value, b.value));
}

function equalConstants(a, b) {
    if (a instanceof Const && b instanceof Const) {
        return a.value === b.value;
    }
    return false;
}

function Const(value) {
    this.value = value;

    Object.freeze(this);
}

Const.prototype.evaluate = function () {
    return this.value;
};

Const.prototype.toString = function () {
    return this.value.toString();
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
        // console.log(a, b);
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
        return new Multiply(res0, res1);
    }
);

var Divide = Operation(
    function (a, b) {
        // console.log(a, b);
        return a / b;
    },
    "/",
    function (c) {
        return new Divide(
            new Subtract(
                new Multiply(this.args[0].diff(c), this.args[1]),
                new Multiply(this.args[0], this.args[1].diff(c))
            ),
            new Multiply(this.args[1], this.args[1])
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
        if (res instanceof Const) {
            return new Const(-res.value);
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
        return this;
    }
);

var Sign = Operation(
    function (a) {
        return a >= 0 ? +1 : -1;
    },
    "sign",
    function (c) {
        return 0;
    },
    function () {
        //return new Const(Math.sign(this.args[0].evaluate()));//TODO
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
            new Sign(this.args[0])
        )
    },
    function () {
        return this
    }//TODO
);

var Sqrt = Operation(
    function (a) {
        return Math.sqrt(Math.abs(a));
    },
    "sqrt",
    function (c) {
        return new Divide(
            new Abs(this.args[0]).diff(c),
            new Multiply(
                new Const(2),
                this
            )
        )
    },
    function () {
        return this;
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
                    new Ln(this.args[0])
                )
            )
        )
    },
    function () {
        return this;
    }
);

var Ln = Operation(
    function (a) {
        return Math.log(Math.abs(a));
    },
    "ln",
    function (c) {
        return new Divide(
            new Abs(this.args[0]).diff(c),
            new Abs(this.args[0])
        )
    }
);

var Log = Operation(
    function (a, b) {
        return Math.log(Math.abs(b)) / Math.log(Math.abs(a));
    },
    "log",
    function (c) {
        return new Divide(
            new Ln(this.args[1]),
            new Ln(this.args[0])
        ).diff(c)
    },
    function () {
        return this;
    }
);

function creator(operation) {
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

    //console.log(arr);
    function step(stack, token) {

        function apply(f, n) {
            var args = stack.splice(-n, n);
            stack.push(creator(f).apply(null, args));
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
    var expr = parse('x y + sqrt').diff('x');
    console.log(expr.toString());
    expr = expr.simplify()
    console.log(expr.toString());
}

// test();