(defn constructor [ctor prototype]
  (fn [& args]
    (let [map (apply ctor {:prototype prototype} args)]
      (let [me (fn [key & args] (apply key map args))]
        me))))

(defn proto-get [obj key]
  (cond
    (contains? obj key) (obj key)
    :else (proto-get (:prototype obj) key)
    ))

(defn field [key]
  (fn [name] (proto-get name key)))

(defn proto-call [this key & args]
  (apply (proto-get this key) (cons this args)))

(defn method [key]
  (fn [this & args] (apply proto-call this key args)))

;------------------------------

(def _evaluate (method :evaluate))
(def _toString (method :toString))
(def _diff (method :diff))


(defn evaluate [e vars] (e _evaluate vars))
(defn toString [e] (e _toString))
(defn diff [e var] (e _diff var))

;------------------------------

(def _value (field :value))
(def _getValue (partial _value))

(declare Constant)
;(declare Constant-assoc)

(def ConstantPrototype
  {
   :getValue (partial _value)
   :evaluate (fn [this vars] (_getValue this))
   :toString (fn [this] (format "%.1f" (double (_getValue this))))
   :diff     (fn [this var] (Constant 0))
   })

(defn Constant-assoc [this value]
  (assoc this
    :value value))

(def Constant (constructor Constant-assoc ConstantPrototype))

;------------------------------

(def _name (field :name))
(def _getName (method :getName))

(defn Variable-assoc [this name]
  (assoc this
    :name name))

(def VariablePrototype
  {:getName  (partial _name)
   :evaluate (fn [this vars] (get vars (_getName this)))
   :toString (fn [this] (_getName this))
   :diff     (fn [this var] (if (= var (_getName this))
                              (Constant 1)
                              (Constant 0)
                              ))
   })

(def Variable (constructor Variable-assoc VariablePrototype))

;------------------------------

(def _args (field :args))
(def _getArgs (method :getArgs))
(def _op (field :op))
(def _symbol (field :symbol))

(defn Operation-assoc [this op symbol diff & args]
  (assoc this
    :op op
    :symbol symbol
    :diff diff
    :args args
    ))

(def OperationPrototype
  {
   :toString (fn [this] (str "(" (_symbol this) " " (clojure.string/join " " (map toString (_getArgs this))) ")"))
   :evaluate (fn [this vars]
               (apply (_op this) (map (fn [x] (evaluate x vars)) (_getArgs this)))
               )
   :getArgs  (partial _args)
   })

(def Operation (constructor Operation-assoc OperationPrototype))

;------------------------------

(def Add
  (partial Operation
           +
           "+"
           (fn [this var] (apply Add (map (fn [e] (diff e var)) (_getArgs this))))
           ))

(def Subtract
  (partial Operation
           -
           "-"
           (fn [this var] (apply Subtract (map (fn [e] (diff e var)) (_getArgs this))))
           ))

(def Multiply
  (partial Operation
           *
           "*"
           (fn [this var] (let [u (nth (_getArgs this) 0)
                                v (nth (_getArgs this) 1)]
                            (Add
                              (Multiply v (diff u var))
                              (Multiply u (diff v var))
                              )))
           ))

(def Divide
  (partial Operation
           (fn [^double x ^double y] (/ x y))
           "/"
           (fn [this var] (let [u (nth (_getArgs this) 0)
                                v (nth (_getArgs this) 1)]
                            (Divide
                              (Subtract
                                (Multiply v (diff u var))
                                (Multiply u (diff v var))
                                )
                              (Multiply v v)
                              )))
           ))

;------------------------------

(def _arg (field :arg))
(def _getArg (method :getArg))

(defn Function-assoc [this op symbol diff arg]
  (assoc this
    :op op
    :symbol symbol
    :diff diff
    :arg arg
    ))

(def FunctionPrototype
  {
   :toString (fn [this] (str "(" (_symbol this) " " (toString (_getArg this)) ")"))
   :evaluate (fn [this vars]
               ((_op this) (evaluate (_getArg this) vars))
               )
   :getArg   (partial _arg)
   })

(def Function (constructor Function-assoc FunctionPrototype))

;------------------------------

(def Negate
  (partial Function
           -
           "negate"
           (fn [this var] (Negate (diff (_getArg this) var)))
           ))

(declare Sin)
(declare Cos)


(def Sin
  (partial Function
           (fn [x] (Math/sin x))
           "sin"
           (fn [this var]
             (let [u (_getArg this)]
               (Multiply
                 (Cos u)
                 (diff u var)
                 )))
           ))

(def Cos
  (partial Function
           (fn [x] (Math/cos x))
           "cos"
           (fn [this var]
             (let [u (_getArg this)]
               (Multiply
                 (Negate (Sin u))
                 (diff u var)
                 )))
           ))

(def symbol-to-binary-operation
  {
   '+      Add
   '-      Subtract
   '*      Multiply
   '/      Divide
   'negate Negate
   'sin    Sin
   'cos    Cos
   })

(defn parseObject [s]
  (defn parse [e]
    (cond
      (symbol? e) (Variable (str e))
      (number? e) (Constant e)
      :else (let [op (first e) args (rest e)] (apply (get symbol-to-binary-operation op) (map parse args)))
      ))
  (parse (read-string s)))

(defn in? [coll] (fn [x] (.contains coll x)))

(def start-predicates
  (vector
    (in? ['+ '-])
    (in? ['* '/])
    ))

(defn parse-recur [lst predicates]
  (if (empty? predicates)
    (
      let [x (first lst)]
      (if (number? x)
        (Constant x)
        (Variable x)
        ))
    (
      let [expr (partition-by (first predicates) lst)
           calc (fn [expr] (if (list? (first expr))
                             (parse-recur (first expr) start-predicates)
                             (parse-recur expr (rest predicates))))]
      ;(println lst expr)
      (reduce
        (fn [acc pair]
          (
            let [op (get symbol-to-binary-operation (first (first pair)))
                 rhs (calc (last pair))]
            (op acc rhs)
            ))
        (calc (first expr)) (partition 2 (rest expr)))
      )
    ))

(defn parseObjectInfix [s]
  (parse-recur
    (read-string (str "(" s ")"))
    start-predicates
    )
  )

(defn check [s] (
                  let [expr (parseObjectInfix s)]
                  (println (toString expr))
                  ))

;(check "1")
;(check "10 + 20")
;(check "100    +      200 - 3")
;(check "1.0 + 2.0 * 3.0 - 4.0")
;(check "1 / 2 *      3")
;(check "1 * 2 * 3 * 4 * 5")
;(check "1 - 2 - 3 - 4 - 5")
;(check "1 / 2 / 3 / 4 / 5 / 6")
;(check "1 - 2 * (3 + 4) + 8")
;(check "1 / (2 + 3) - (3 + 5) / (6 + 7) + (1 * 1)")
;(check "1 + 2")
;(check "(1 + 2)")
;(check "1000 * 0.004 / 2")
;(check "((()( (1 * (3 / 2) + 5) - 1000 * 0.504 / 2)))")

