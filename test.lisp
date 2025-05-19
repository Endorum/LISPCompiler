#include "stdlib.lisp"

;; reads a character at index "index"
(defun readchar (ptr index)
  (deref (+ ptr index))
)

;; works with strings or arrays on the .section data or in the heap
(defun memcpy (ptrDst ptrSrc num )

  (let i 0)

  (while (< i num)
    (let ch (readchar ptrSrc i))
    (setchar ptrDst i ch)
    (+= i 1)
  )

  ptrDst
  
)

;; creating a new string on the heap and storing an init in it
(defun string (init)
  (let len (strlen init))
  (let str (malloc (+ len 1))) ;; +1 to store the null terminator
  (memcpy str init len)
  (setchar str len 0) ;; save the null terminator
  str
)



(defun strcat (strA strB)
  (let lenA (strlen strA))
  (let lenB (strlen strB))
  (let res_len (+ lenA lenB))

  (let res_str (malloc (+ res_len 1)))

  (memcpy res_str strA lenA)
  (memcpy (+ res_str lenA) strB lenB)

  (setchar res_str res_len 0)
  res_str
)

(defun setint (ptr index value)
  ;; writes 4 bytes info ptr: value & 0xFF, (value >> 8) & 0xFF, ...
  (setchar ptr (+ 0 index) (& value 255))
  (setchar ptr (+ 1 index) (& (>> value 8) 255 ))
  (setchar ptr (+ 2 index) (& (>> value 16) 255 ))
  (setchar ptr (+ 3 index) (& (>> value 24) 255 ))
  ptr
)

(defun getint (ptr index)
  (let b0 (deref (+ ptr (+ index 0))))
  (let b1 (deref (+ ptr (+ index 1))))
  (let b2 (deref (+ ptr (+ index 2))))
  (let b3 (deref (+ ptr (+ index 3))))

  (+ b0 
    (<< b1 8)
    (<< b1 16)
    (<< b1 24) )
)

(defun alloc_string (len)
  ;; allocate space for meta data (8 bytes = 4 bytes length + 4 bytes capacitry) + string + null
  (let total_size (+ len 9))
  (let ptr (malloc total_size)) 
  (setint ptr 0 len)      ;; set length at offset 0
  (setint ptr 4 len)      ;; set capacity at offset 4
  (let str_ptr (+ ptr 8)) ;; real start of string
  (setchar str_ptr len 0) ;; null terminator
  str_ptr
)

(defun get_length (str)
  (- str 8)
  (getint str 0)
)

(defun get_capacity (str)
  (- str 4)
  (getint str 0)
)

(defun set_length (str len)
  (- str 8)
  (setint str 0 len)
  (+ str 8)
)

(defun append_char__grow_string (str len cap ch)
    (let new_cap (* cap 2))
    (let new_ptr (alloc_string new_cap))
    (memcpy new_ptr str len)
    (set_length new_ptr len)
    (setchar new_ptr len ch)
    (setchar new_ptr (+ len 1) 0)
    (set_length new_ptr (+ len 1))
    new_ptr
)

(defun append_char__else (str len ch)
    (setchar str len ch)
    (setchar str (+ len 1) 0)
    (set_length str (+ len 1))
    str
)

(defun append_char (str ch)
  (let len (get_length str))
  (let cap (get_capacity str))

  (if (>= len cap)
    ;; grow string
    (append_char__grow_string str len cap ch)

    ;; else
    (append_char__else str len ch)
    
  )
)


(defun main()
  ;; (let strA (string "Hello "))
  ;; (let newStrA )
  (alloc_string 255)
)
