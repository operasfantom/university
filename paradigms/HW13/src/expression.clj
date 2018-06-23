(def constant constantly)
(defn variable [name] (fn [maps] (get maps name)))

(defn multi-operation [f]
  (fn [& args]
    (fn [maps]
      (apply f (map (fn [e] (e maps)) args))
      )
    ))

(defn unary-operation [f]
  (fn [arg]
    (fn [maps]
      (f ((fn [e] (e maps)) arg))
      )
    ))

(def add (multi-operation +))
(def subtract (multi-operation -))
(def multiply (multi-operation *))
(def divide (multi-operation (fn [^double x ^double y] (/ x y))))
(def negate (unary-operation -))

(def sinh (unary-operation (fn [x] (Math/sinh x))))
(def cosh (unary-operation (fn [x] (Math/cosh x))))

(def symbol-to-binary-operation
  {
   '+      add
   '-      subtract
   '*      multiply
   '/      divide
   'negate negate
   'sinh   sinh
   'cosh   cosh
   })

(defn parseFunction [s]
  (defn parse [e]
    (cond
      (symbol? e) (variable (str e))
      (number? e) (constant e)
      :else (let [op (first e) args (rest e)] (apply (get symbol-to-binary-operation op) (map parse args)))
      ))
  (parse (read-string s)))