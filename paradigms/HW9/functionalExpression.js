// Node.js compatible runner

"use strict";

function cnst(x) {
    return function () {
        return x
    }
}

function variable(name) {
    return function (x) {
        if (name === "x") {
            return x;
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
            // console.log(arr)
            var result = arr.reduce(operation);
            // console.log("result:", result)
            return result;
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

function test() {
    /*var expr = subtract(
        add(
            cnst(2),
            variable("x")
        ),
        cnst(3)
    );

    console.log(expr(5));
    */

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
    var arr = s.split(' ');
    //console.log(arr);
    function step(stack, token) {

        function apply_binary(f) {
            let x1 = stack.pop();
            let x0 = stack.pop();
            stack.push(f(x0, x1));
        }

        switch (token) {
            case "+":
                apply_binary(add);
                break;
            case "-":
                apply_binary(subtract);
                break;
            case "*":
                apply_binary(multiply);
                break;
            case "/":
                apply_binary(divide);
                break;
            default:
                //x
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

var result = parse("x x 2 - * x * 1 +")(5);
console.log(result);
