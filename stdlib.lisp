(defun strlen_internal (str n)
    (if (= (deref str) 0)  
        n
        (strlen_internal (+ str 1) (+ n 1))
    )
)

(defun strlen (str)
    (strlen_internal str 0)
)


(defun digitToString (x)
    (cond 
        ( (= x 0) (+ 0 48))
        ( (= x 1) (+ 1 48))
        ( (= x 2) (+ 2 48))
        ( (= x 3) (+ 3 48))
        ( (= x 4) (+ 4 48))
        ( (= x 5) (+ 5 48))
        ( (= x 6) (+ 6 48))
        ( (= x 7) (+ 7 48))
        ( (= x 8) (+ 8 48))
        ( (= x 9) (+ 9 48))
        ( else "ERROR" )
    )

)

(defun reverse_str (str)
    (let len (strlen str))
    (let out "..........")
    (let i 0)
    (while (< i len)
        (let c (
            deref (+ 
                str 
                (- (- 
                    len 
                    i
                ) 1)
                    )
                )
        ) ;; maybe digitToString is necessary
        (setchar out i c)
        (set i (+ i 1))
    )
    out
)

;; itoa
;; returns a string ptr
;; itoa (int value, int base)
(defun itoa (num base)
    (let tmp "..........") ;; pre allocated result
    (let i 0)

    (while (> num 0)
        (let digit (mod num base))
        (let ch (digitToString digit))
        (setchar tmp i ch) ;; set a character in tmp
        (+= i 1)
        (/= num base)
    )

    (reverse_str tmp)
)