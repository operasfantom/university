(defn equal-lengths? [& args] (apply = (mapv count args)))
(defn vectors-of-numbers? [& args] (apply (every-pred vector? (fn [x] (every? number? x))) args))
(defn matrixes-of-numbers? [& args] (and
                                      (every? vector? args)
                                      (equal-lengths? args)
                                      (apply (every-pred (fn [x] (every? vectors-of-numbers? x))) args)
                                      ))

(defn folding-vectors? [& args] (and
                                  (apply vectors-of-numbers? args)
                                  (apply equal-lengths? args)
                                  ))
(defn folding-matrixes? [& args] (and
                                   (apply matrixes-of-numbers? args)
                                   (apply equal-lengths? args)
                                   ))
(defn multiplying-matrixes? [x, y] (and
                                     (matrixes-of-numbers? x y)
                                     (= (count (x 0)) (count y))
                                     ))

(defn numbers? [& args] (every? number? args))

(defn equivalent-matrixes? [& args] (and
                                      (apply matrixes-of-numbers? args)
                                      (apply equal-lengths? args)
                                      (= (mapv (fn [x] (mapv count x)) args))
                                      ))
;------------------------------------------------------------------------------

(defn vector-operation [f, args]
  {:pre [(apply folding-vectors? args)]}
  (apply mapv f args))

(defn v+ [& args] (vector-operation + args))
(defn v- [& args] (vector-operation - args))
(defn v* [& args] (vector-operation * args))

(defn scalar [& args]
  {:pre [(apply folding-vectors? args)]}
  (reduce + (apply v* args)))

(defn vect [& args]
  {:pre [(and
           (apply folding-vectors? args)
           (every? (fn [x] (<= (count x) 3)) args)
           )]}
  (reduce
    (fn [a, b] (case (count a)
                 2 (vector (* (a 0) (b 1), * (a 1) (b 0)))
                 3 (vector (- (* (a 1) (b 2)) (* (a 2) (b 1)))
                           (- (* (a 2) (b 0)) (* (a 0) (b 2)))
                           (- (* (a 0) (b 1)) (* (a 1) (b 0)))
                           )))
    args)
  )

(defn v*s [x & args]
  {:pre [(and
           (vectors-of-numbers? x)
           (apply numbers? args)
           )]}
  (reduce (fn [v, s] (mapv (fn [x] (* x s)) v)) x args))

(defn matrix-operation [f, args]
  {:pre [(apply folding-matrixes? args)]}
  (apply mapv f args))

(defn m+ [& args] (matrix-operation v+ args))
(defn m- [& args] (matrix-operation v- args))
(defn m* [& args] (matrix-operation v* args))

(defn transpose [x]
  {:pre [(matrixes-of-numbers? x)]}
  (apply mapv vector x))

(defn m*s [x & args]
  {:pre [(and
           (matrixes-of-numbers? x)
           (apply numbers? args)
           )]}
  (reduce (fn [m, s] (mapv (fn [x] (v*s x s)) m)) x args))

(defn m*v [x & args]
  {:pre [(and
           (matrixes-of-numbers? x)
           (apply folding-vectors? args)
           )]}
  (reduce (fn [m, v] (mapv (fn [x] (scalar x v)) m)) x args))

(defn step [x, y]
  {:pre [(and
           (matrixes-of-numbers? x y)
           (multiplying-matrixes? x y)
           )]}
  (mapv (fn [i] (mapv (fn [j] (scalar i j)) (transpose y))) x))

(defn m*m [& args] (reduce step args))

(defn depth [t]
  (
    if (number? t)
    (list)
    (
      let [a, (map depth t)]
      (assert (= a))
      (cons (count a) (nth a 0))
      )
    ))

(defn shape [f, args]
  {:pre [(or
         (apply numbers? args)
         (not-any? number? args))
         ]}
  (
    if (apply numbers? args)
    (apply f args)
    (apply mapv (fn [& x] (shape f x)) args)
    ))

(defn s+ [& args] (shape + args))
(defn s- [& args] (shape - args))
(defn s* [& args] (shape * args))

(defn depth [t] (
                  if (number? t)
                  (list)
                  (
                    let [a, (map depth t)]
                    (assert (apply = a))
                    (cons (count a) (nth a 0))
                    )
                  ))

(defn broadcast [& args]
  (
    let [d, (mapv reverse (mapv depth args))
         l, (apply max-key count d)
         ]
    (mapv (fn [x] (reduce (fn [t, dim] (apply vector (repeat dim t))) x (drop (count (depth x)) l))) args)
    ))

(defn b+ [& args] (apply s+ (apply broadcast args)))
(defn b- [& args] (apply s- (apply broadcast args)))
(defn b* [& args] (apply s* (apply broadcast args)))

;(println (s- 1))
;(println (apply - (vector 1)))