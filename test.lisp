
;(defun digit_to_string (x)
;  (cond
;      ((= x 0) "0")
;      ((= x 1) "1")
;      ((= x 2) "2")
;      ((= x 3) "3")
;      ((= x 4) "4")
;      ((= x 5) "5")
;      ((= x 6) "6")
;      ((= x 7) "7")
;      ((= x 8) "8")
;      ((= x 9) "9")
;      (else "ERROR")
;  )
;)

;(defun greater (x y)
;    (if (> x y) "yes" "no")
;)


(defun test ()
  (let x 5)
  (let y 6)
  (if (= (+ x y) 11) "yes" "no")
)

(defun main()
    (print (test))
)