(def constant constantly)
(defn variable [name] (fn [maps] (get maps name)))

(defn operation [f]
  (fn [& args]
    (fn [maps]
      (apply f (map (fn [e] (e maps)) args))
      )
    ))

(def add (operation +))
(def subtract (operation -))
(def multiply (operation *))
(def divide (operation (fn [^double x ^double y] (/ x y))))
(def negate subtract)

(def sinh (operation (fn [x] (Math/sinh x))))
(def cosh (operation (fn [x] (Math/cosh x))))

(def square (operation (fn [x] (* x x))))
(def sqrt (operation (fn[x] (Math/sqrt (Math/abs x)))))

(def symbol-to-operation
  {
   '+ add
   '- subtract
   '* multiply
   '/ divide
   'negate negate
   'sinh sinh
   'cosh cosh
   'square square
   'sqrt sqrt
   })

(defn parseFunction [s]
  ((fn parse [e]
     (cond
       (symbol? e) (variable (str e))
       (number? e) (constant e)
       :else ((fn [op args] (apply (get symbol-to-operation op) (map parse args))) (first e) (rest e))
       )) (read-string s)))


;(println ((parseFunction "         (+ x z) ") {"x" 2.4 "z" 1.5}))
;(println ((parseFunction "(add (variable \"x\") (constant 2.0))") {"x" 0.0}))
;(def expr
  ;(add (variable "x") (constant 2.0))
  ;(variable "x")
  ;(constant 2.0)
  ;)
;(println ((parseFunction "         (/ 1 0) ") {"x" 2.4 "z" 1.5}))
;(println (expr {"x" 1.0}))







