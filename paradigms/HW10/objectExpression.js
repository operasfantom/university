"use strict";

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

function Operation(operation, symbol) {
    function Constructor() {
        this.args = arguments;
        this.getArgs = function () {
            return this.args;
        };
        this.operation = operation;
        this.symbol = symbol;
    }

    Object.setPrototypeOf(Constructor.prototype, Expression.prototype);
    Object.freeze(Constructor);
    return Constructor;
}

function Const(value) {
    this.value = value;
    this.evaluate = function () {
        return this.value;
    };
    this.toString = function () {
        return this.value.toString();
    };
    Object.freeze(this);
}

function Variable(name) {
    this.name = name;
    this.evaluate = function (x, y, z) {
        if (name === "x") {
            return x;
        } else if (name === "y") {
            return y;
        } else if (name === "z") {
            return z;
        } else {
            throw Error(name + "is wrong name of Variable")
        }
    };
    this.toString = function () {
        return this.name;
    };
    Object.freeze(this);
}

var Add = Operation(
    function (a, b) {
        return a + b;
    },
    "+"
);

var Subtract = Operation(
    function (a, b) {
        return a - b;
    },
    "-"
);

var Multiply = Operation(
    function (a, b) {
        return a * b;
    },
    "*"
);

var Divide = Operation(
    function (a, b) {
        return a / b;
    },
    "/"
);

var Negate = Operation(
    function (a) {
        return -a;
    },
    "negate"
);

function test() {
    var expr = new Negate(new Variable("z"));
    assertEquals("z negate", expr.toString());
}

// test();