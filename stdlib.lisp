;; malloc(int:size) -> ptr, allocates a block of size size bytes of memory returning a pointer to the beg
;; itoa(num base) -> str integer to string with base
;; atoi(str) -> int string to integer
;; strlen(str) -> int returns length of string
;; strcmp(strA strB) return 0 if they are the same, not 0 if they are different
;; strncmp(strA strB amount) -> 0/1 compares amount of characters 
;; strcat(strA strB) -> strA + strB
;; memcpy(dst src num) -> dst, copies num amount of bytes from src to dst
;; memset(ptr int:value num) -> ptr, sets the first num of bytes in ptr to the value value

;; (defun malloc (size)

;; )


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
    (if (< x 10) (+ x 48) (+ x 55)) ;; after 0..9 comes A B etc
)

(defun reverse_str (str)
    (let len (strlen str))
    (let out "..........")
    (let i 0)
    (while (< i len)
        (let c ( 
            deref (+ str (- (- len i ) 1) )
            )
        )
        (setchar out i c)
        (set i (+ i 1))
    )
    out
)

(defun printLn (str)
    (print str)
    (print "\n")
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