;; ******************************************************************
;;
;;    Copyright (C) 1995 Blue Marsh Softworks -- All rights reserved
;;
;; PROJECT:      COMP 410 Assignments
;; MODULE:       AddLists
;; FILE:         addlist.lsp
;;
;; AUTHOR:       Nathan Fiedler
;;
;; REVISION HISTORY:
;;      Name   Date     Description
;;      ----   ----     -----------
;;       NF    02/11/95 Initial version
;;
;; DESCRIPTION:
;;      This LISP program defines two functions. Both of them add
;;      two lists together. The first one adds the lists in a
;;      recursive manner, while the second uses an iterative loop.
;;
;; ******************************************************************

;; ******************************************************************
;;                 ADD-LIST-RECURSIVE
;; ******************************************************************
;; SYNOPSIS:     Given two lists of numbers this function will add
;;               each element and return a list of the results.
;;               This is the recursive version.
;;               If the lists are not of equal size, or if any of
;;               the elements are not numbers, then add-list-recursive
;;               will return nil.
;; CALLED BY:    User
;; PARAMETERS:   ( l1 l2 )  Two lists of numbers.
;; RETURNS:      ( list )  The list containing the results of adding
;;               each element of each list.
;; SIDE EFFECTS:
;; STRATEGY:     First check that the parameters are in fact lists,
;;               then make sure the lengths are the same. Then use
;;               use a recursive call to add the lists.
;; ******************************************************************

(defun add-list-recursive ( l1 l2 )
  ;; Check that the forms are lists.
  (check-type l1 list)
  (check-type l2 list)
  ;; Make sure lists are of equal size.
  (if (= (length l1) (length l2))
    ;; Base case when list is empty.
    (if (null l1)
         nil
        ;; Else, check that the elements are numbers
        ;; and then add them.
        (if  (and (typep (first l1) 'number)
                  (typep (first l2) 'number))
          (cons (+ (first l1) (first l2))
                (add-list-recursive (rest l1) (rest l2)))))))

;; ******************************************************************
;;                 ADD-LIST-ITERATE
;; ******************************************************************
;; SYNOPSIS:     Given two lists of numbers this function will add
;;               each element and return a list of the results.
;;               This is the iterative version.
;;               If the lists are not of equal size, add-list-iterate
;;               will return nil. If the lists contain a non-number,
;;               then the respective element in both lists will be
;;               skipped.
;;               For example: (add-list-iterate '(1 a 3) '(1 2 3))
;;               will return (1 6).
;; CALLED BY:    User
;; PARAMETERS:   ( l1 l2 )  Two lists of numbers.
;; RETURNS:      ( list )  The list containing the results of adding
;;               each element of each list.
;; SIDE EFFECTS:
;; STRATEGY:     First check that the parameters are in fact lists,
;;               then make sure the lengths are the same. Then use
;;               use a dotimes loop to add the lists.
;; ******************************************************************

(defun add-list-iterate ( l1 l2 )
  ;; Check that the forms are lists.
  (check-type l1 list)
  (check-type l2 list)
  ;; Make sure lists are of equal size.
  (if (= (length l1) (length l2))
    (let (result)
      ;; Start loop to add the lists.
      (dotimes (count (length l1) result)
        (if (and (typep (nth count l1) 'number)
                 (typep (nth count l2) 'number))
          ;; Add the two elements are append result to 'result' list.
          (setf result (append result (list (+ (nth count l1)
                                               (nth count l2))))))))))
