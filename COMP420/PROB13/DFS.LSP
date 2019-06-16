
#|----------------------------------------------------------------------------
Artificial Intelligence, Second Edition
Elaine Rich and Kevin Knight
McGraw Hill, 1991

This code may be freely copied and used for educational or research purposes.
All software written by Kevin Knight.
Comments, bugs, improvements to knight@cs.cmu.edu
----------------------------------------------------------------------------|#

#|----------------------------------------------------------------------------
			DEPTH-FIRST SEARCH
			    "dfs.lisp"
----------------------------------------------------------------------------|#

;; --------------------------------------------------------------------------
;; Function DFS does a depth-first search from the start state. If a goal
;; state is found, it returns a solution path.  DFS can easily get stuck
;; in an infinite loop, since it may enter the same state twice along the 
;; same search path.

(defun dfs (start &optional verbose)
  (let ((result (dfs-1 start verbose)))
    (if (null result) "No solution." result)))

(defun dfs-1 (start verbose)
  (when verbose (format t "Expanding node ~d~%" start))
  (cond ((goal-state? start) (list start))
	(t  
	 (do ((succs (expand start) (cdr succs))
	      (solution-found nil))
	     ((or solution-found (null succs))
	      (if solution-found (cons start solution-found) nil))
           (setq solution-found (dfs-1 (car succs) verbose))))))


;; --------------------------------------------------------------------------
;; Function DFS-AVOID-LOOPS does a depth-first search from the start state,
;; but never enters a state which it has already entered along the current 
;; search path. It avoids loops by remembering all of the ancestors of a 
;; given state.
;; 
;; Function DFS-WITH-CUTOFF takes an additional parameter, DEPTH-CUTOFF,
;; and performs the same search as DFS-AVOID-LOOPS, but never searches 
;; to a depth beyond the cutoff.

(defun dfs-avoid-loops (start &optional verbose)
  (let* ((parents nil)
 	 (depth-cutoff *infinity*)
         (result (dfs-avoid-loops-1 start parents depth-cutoff verbose)))
    (if (null result) "No solution." result)))

(defun dfs-with-cutoff (start depth-cutoff &optional verbose)
  (let* ((parents nil)
         (result (dfs-avoid-loops-1 start parents depth-cutoff verbose)))
    (if (null result) "No solution." result)))


(defun dfs-avoid-loops-1 (start parents depth-cutoff verbose)
  (when verbose (format t "Expanding node ~d~%" start))
  (cond ((goal-state? start) (list start))
	((= depth-cutoff 0) nil)	; decrease depth-cutoff until it is 0
	(t  (let ((all-succs (expand start)))
	       (do ((succs (remove-dfs-parents all-succs parents)
			   (cdr succs))
		    (solution-found nil))
		   ((or solution-found (null succs))
		    (if solution-found (cons start solution-found) nil))
		 (setq solution-found (dfs-avoid-loops-1 (car succs)  
				 		   	 (cons start parents)
							 (1- depth-cutoff)
					           	 verbose)))))))

(defun remove-dfs-parents (all-succs parents)
  (mapcan #'(lambda (succ)
		(if (member succ parents :test #'eq-states)
		    nil
		    (list succ)))
	  all-succs))
