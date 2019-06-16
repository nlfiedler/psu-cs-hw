
#|--------------------------------------------------------------------------
Artificial Intelligence, Second Edition
Elaine Rich and Kevin Knight
McGraw Hill, 1991

This code may be freely copied and used for educational or research purposes.
All software written by Kevin Knight.
Comments, bugs, improvements to knight@cs.cmu.edu
--------------------------------------------------------------------------|#

#|--------------------------------------------------------------------------
		            GENERIC DOMAIN
			    "generic.lisp"
-------------------------------------------------------------------------|#

#|-------------------------------------------------------------------------

This file can be used as a template to create new domains.  To create your
own domain, fill in the functions below, adding whatever auxiliary functions
are necessary.  The functions are:

 (goal-state? s)		t if s is a goal state
 (eq-states s1 s2)		are s1 and s2 equal?
 (expand s)			successor states of s
 (hash-state s)			some has value for s
 (print-state s)		print function
 (destroy-state s)		dispose of state structure
 (heuristic s)			heuristic distance between s and the goal
 (generate-problem)		randomly generated start-state
 (cost-of-move s1 s2)		cost of moving from s1 to s2

The important variables are:

 *sample-initial-state*
 *goal-state*

These functions and variables can all be called from an outside search
program.  In fact, these are the functions called by our implementations of
depth-first, breadth-first, hill-climbing, A*, DFID, IDA*, and RTA* search.

For example domains, see "n-puzzle.lisp" and "travel.lisp".

--------------------------------------------------------------------------|#

;; Variable *INFINITY* will be used as the largest possible number.
;; MOST-POSITIVE-FIXNUM is a Lisp symbol that provides it.

(defvar *infinity* most-positive-fixnum)


;; Variable *SAMPLE-INITIAL-STATE* holds a sample initial state.

(defvar *sample-initial-state*)


;; Variable *GOAL-STATE* holds a goal state.  This can be nil if there is no 
;; unique goal state.

(defvar *goal-state*)


(setq *sample-initial-state* nil)

(setq *goal-state* nil)

;; ---------------------------------------------------------------------
;; Functions required by search programs in any domain.


;; Function GOAL-STATE? returns t iff s is a goal state.

(defun goal-state? (s)
  nil)


;; Function EQ-STATES returns t iff s1 and s2 are the same state.

(defun eq-states (s1 s2)
  nil)


;; Function EXPAND takes a state and returns all possible successor states.

(defun expand (s)
  nil)


;; Function PRINT-STATE prints a state.

(defun print-state (s &rest ignore)
  (declare (ignore ignore))
  nil)


;; Function HEURISTIC provides a numerical estimate of the difficulty of 
;; reaching a goal state from s.  The higher the estimate, the more
;; difficulty. 

(defun heuristic (s)
  nil)


;; Function GENERATE-PROBLEM returns a randomly generated start state.

(defun generate-problem ()
  nil)


