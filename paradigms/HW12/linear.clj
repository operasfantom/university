(defn equal-lengths? [& args] (apply == (mapv count args)))
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
                                     (= (count (nth x 0)) (count y))
                                     ))

(defn numbers? [& args] (every? number? args))

(defn equivalent-matrixes? [& args] (and
                                      (apply matrixes-of-numbers? args)
                                      (apply equal-lengths? args)
                                      (== (mapv (fn [x] (mapv count x)) args))
                                      ))
;------------------------------------------------------------------------------

(defn vector-operation [f, args]
  {:pre  [(apply folding-vectors? args)]
   :post [(and
            (vectors-of-numbers? %)
            (apply equal-lengths? % args)
            )]}
  (apply mapv f args))

(defn v+ [& args] (vector-operation + args))
(defn v- [& args] (vector-operation - args))
(defn v* [& args] (vector-operation * args))

(defn scalar [& args]
  {:pre  [(apply folding-vectors? args)]
   :post [(number? %)]}
  (reduce + (apply v* args)))

(defn vect [& args]
  {:pre  [(and
            (apply folding-vectors? args)
            (every? (fn [x] (<= (count x) 3)) args)
            )]
   :post [(and
            (vectors-of-numbers? %)
            (apply equal-lengths? % args)
            )]}
  (reduce
    (fn [a, b] (case (count a)
                 2 (vector (* (nth a 0) (nth b 1), * (nth a 1) (nth b 0)))
                 3 (vector (- (* (nth a 1) (nth b 2)) (* (nth a 2) (nth b 1)))
                           (- (* (nth a 2) (nth b 0)) (* (nth a 0) (nth b 2)))
                           (- (* (nth a 0) (nth b 1)) (* (nth a 1) (nth b 0)))
                           )))
    args)
  )

(defn v*s [x & args]
  {:pre  [(and
            (vectors-of-numbers? x)
            (apply numbers? args)
            )]
   :post [(and
            (vectors-of-numbers? %)
            (equal-lengths? % x)
            )]}
  (reduce (fn [v, s] (mapv (fn [x] (* x s)) v)) x args))

(defn matrix-operation [f, args]
  {:pre  [(apply folding-matrixes? args)]
   :post [(and
            (matrixes-of-numbers? %)
            (apply equivalent-matrixes? % args)
            )]}
  (apply mapv f args))

(defn m+ [& args] (matrix-operation v+ args))
(defn m- [& args] (matrix-operation v- args))
(defn m* [& args] (matrix-operation v* args))

(defn transpose [x]
  {:pre  [(matrixes-of-numbers? x)]
   :post [(and
            (matrixes-of-numbers? %)
            (multiplying-matrixes? x %)
            (multiplying-matrixes? % x)
            )]}
  (apply mapv vector x))

(defn m*s [x & args]
  {:pre  [(and
            (matrixes-of-numbers? x)
            (apply numbers? args)
            )]
   :post [(and
            (matrixes-of-numbers? %)
            (equivalent-matrixes? x %)
            )]}
  (reduce (fn [m, s] (mapv (fn [x] (v*s x s)) m)) x args))

(defn m*v [x & args]
  {:pre  [(and
            (matrixes-of-numbers? x)
            (apply folding-vectors? args)
            )]
   :post [(and
            (vectors-of-numbers? %)
            (= (count %) (count x))                         ;?
            )]}
  (reduce (fn [m, v] (mapv (fn [x] (scalar x v)) m)) x args))

(defn step [x, y]
  {:pre  [(and
            (matrixes-of-numbers? x y)
            (multiplying-matrixes? x y)
            )]
   :post [(and
            (matrixes-of-numbers? %)
            (= (count x) (count %))
            (= (count (transpose y)) (count (transpose %)))
            )]}
  (mapv (fn [i] (mapv (fn [j] (scalar i j)) (transpose y))) x))

(defn m*m [& args] (reduce step args))

(defn shape [f, args] (
                        if (apply numbers? args)
                        (apply f args)
                        (apply mapv (fn [& x] (shape f x)) args)
                        ))

(defn s+ [& args] (shape + args))
(defn s- [& args] (shape - args))
(defn s* [& args] (shape * args))
