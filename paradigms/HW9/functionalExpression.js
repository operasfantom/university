// Node.js compatible runner

"use strict";

var x = variable("x");
var y = variable("y");
var z = variable("z");
var pi = cnst(Math.PI);
var e = cnst(Math.E);

function cnst(x) {
    return function () {
        return x
    }
}

function variable(name) {
    return function (x, y, z) {
        if (name === "x") {
            return x;
        } else if (name === "y") {
            return y;
        } else if (name === "z") {
            return z;
        } else {
            throw Error(name)
        }
    }
}

function evaluate(operation) {
    return function () {
        // console.log("operation:", operation)
        var elements = arguments;
        // console.log("elements:", elements);
        return function () {
            // console.log(arguments)
            var arr = [].slice.call(elements);
            // console.log(arr)
            var parameters = arguments;
            arr = arr.map(function (f) {
                return f.apply(null, parameters)
            });
            return operation.apply(null, arr);
        }
    }
}

var add = evaluate(function (a, b) {
    return a + b;
});

var subtract = evaluate(function (a, b) {
    return a - b;
});

var multiply = evaluate(function (a, b) {
    return a * b;
});

var divide = evaluate(function (a, b) {
    return a / b;
});

var negate = evaluate(function (a) {
    return -a;
});

var min3 = evaluate(function (a, b, c) {
    return Math.min(a, b, c)
});

var max5 = evaluate(function (a, b, c, d, e) {
    return Math.max(a, b, c, d, e)
});

var min = evaluate(Math.min);

var max = evaluate(Math.max);

function test() {
    var expression = add(
        subtract(
            multiply(
                variable("x"),
                variable("x")
            ),
            multiply(
                cnst(2),
                variable("x")
            )
        ),
        cnst(1)
    );

    function calc(x) {
        return x * x - 2 * x + 1;
    }

    for (var x = 0; x <= 10; x++) {
        console.log("x =", x, "expected:", calc(x), "actual:", expression(x))
    }
}

function parse(s) {
    var arr = s.split(/\s+/);

    //console.log(arr);
    function step(stack, token) {

        function apply(f, n) {
            var args = stack.splice(-n, n)
            stack.push(f.apply(null, args));
        }

        switch (token) {
            case "":
                break;
            case "+":
                apply(add, 2);
                break;
            case "-":
                apply(subtract, 2);
                break;
            case "*":
                apply(multiply, 2);
                break;
            case "/":
                apply(divide, 2);
                break;
            case "negate":
                apply(negate, 1);
                break;
            case "pi":
                stack.push(pi);
                break;
            case "e":
                stack.push(e);
                break;
            default:
                if (token.slice(0, 3) === "min") {
                    var n = parseInt(token.slice(3, token.length));
                    apply(min, n);
                    break;
                }
                if (token.slice(0, 3) === "max") {
                    var n = parseInt(token.slice(3, token.length));
                    apply(max, n);
                    break;
                }
                if (isNaN(token)) {
                    stack.push(variable(token));
                } else {
                    stack.push(cnst(parseInt(token)))
                }
        }
        return stack;
    }

    return arr.reduce(step, [])[0]
}

// test();

// var result = parse("x x 2 - * x * 1 +")(5);
// console.log(result);

// var result = parse('x y z min3')(1, 0, 0);
// console.log(result);

