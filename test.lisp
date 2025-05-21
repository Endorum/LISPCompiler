;; #include "stdlib.lisp"


(defun main()
  (let a 5)
  (let b 6)

  (if (> a b)
    (+= a b)
    (-= a b)
  )
)