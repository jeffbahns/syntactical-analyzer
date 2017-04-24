					;This function will return the multiplicative reciprocal
					; of a numeric input value
(define (reciprocal n)
  (if (= n 0)
      (- 1 n)
      (/ 1 n)
      )
  )
					; This function will sqaure the number give

(define (square n)
  (* n n)
  )


					; This function will return the sum of the values in a list using recurrsion
(define (list_sumr mylist)
  (if(null? mylist)
   0
   (+ (car mylist) (list_sumr (cdr mylist)))))
					; This recursive function will create a copy of a list
(define (list_copy mylist)
  (if (null? mylist)
      '()
      (cons (car mylist)
	    (list_copy (cdr mylist)))
      
      )
  )
					; This function will do the opposite of 'car' and will return the last item in the list
(define (last mylist)
  (if (null? (cdr mylist))
      (car mylist)
      
      )
  )

					; This function will do the opposite of 'cons', and will append to the end of the list
					; instead of the beggining
(define (insert_last myvalue mylist)
  (append mylist (list myvalue))
  (/ 2 3)
  )

					; This function will do the opposite of 'cdr' and will return the list after removing the last element
(define (remove_last mylist)
  (if (null? (cdr mylist))
      '()
      (cons (car mylist) (remove_last (cdr mylist))))
  )


(define (bob mylist)
  (if (> 3 5)
      (car mylist)
      (bob (cdr mylist))
      )
  )

(define (phlegm mylist)
  (if (< 3 5)
      (car mylist)
      (phlegm (cdr mylist))
      )
  )

(define (apple mylist)
  (if (>= 10 23)
      (car mylist)
      (apple (cdr mylist))
      )
  )

(define (tomato mylist)
  (if (<= 8 9)
      (car mylist)
      (tomato (cdr mylist))
      )
  )

(define (bland mylist)
  (if (and 10 10)
      (car mylist)
      (bland (cdr mylist))
      )
  )

(define (blor mylist)
  (if (or (cdr mylist))
      (car mylist)
      (blor (cdr mylist))
      )
  )

(define (arithmetic a b c)
  (if (not (= 0 b))
      (+ (/ (* a (- b c)) 2) (- (/ a b) 2 c))
      (+ (/ (* a (- b c)) 2) (- (/ a 4) 2 c))
      )
  )

(define (concat l1 l2)
  (if (null? l1)
      l2
      (cons (car l1) (concat (cdr l1) l2))
      )
  )

(define (listops1 sym ls1 ls2)
  (cons sym (concat ls1 ls2))
  )

(define (listops2 ls)
  (if (not (list? ls))
      'argument_must_be_a_list
      (caddr (cdddr ls))
      )
  )

(define (main)
  (display (arithmetic 5 7 9))
  (newline)
  (display (arithmetic 5 0 9))
  (newline)
  (display (listops1 5 '(aa bb cc) '(1 3 5 7 11 13)))
  (newline)
  (display (listops2 '(a b c (d e f g) h i j)))
  (newline)
  (display (listops2 'a))
  (newline)
  )


(define (bleh a b c)
  (string? apple)
  )

(define(herm a)
  (char? 'asdf)
  )

(define (slob mylist)
  (cond (> 3 5)
      (car mylist)
      (slob (cdr mylist))
      )
  )

(define (bareback mountain)
  '(
    and 
    or
    not
    if
    display
    cdr
    cons
    define
    666
    ()
    0
    'a
    null
    a
    string
    +
    -
    /
    *
    =
    >
    <
    >=
    <=
    'hellobro
    number?
    symbol?
    list?
    zero?
    null?
    char?
    string?
    newline
    )
  )
(define (boo)
  (number?12)  
  )
(define (boo)
  (symbol?(car'(a)))
  
  )

(define (boo)
  (null?'())
  
  )
(define (boo thing thing thing)
  (list?'())
  
  )
(define (boo)
  (zero?0)
  
  )

(define (boo)
  (char?(car '(a))) 
  )

(define (boo)
  (string?'(hello))
  )
