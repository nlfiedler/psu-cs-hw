;; ------------------------------------------------------------------
;;
;;    Copyright (C) 1995 Blue Marsh Softworks -- All rights reserved
;;
;; PROJECT:      COMP 420 - Artificial Intelligence
;; MODULE:       Missionaries & Cannabals
;; FILE:         MISSCAN.LSP
;;
;; AUTHOR:       Nathan Fiedler
;;
;; REVISION HISTORY:
;;      Name     Date        Description
;;      ----     ----        -----------
;;       NF      02/21/95    Initial version
;;
;; DESCRIPTION:
;;      This is Problem Set 2, Problem #1. This file is based on the
;;      GENERIC.LSP source code by Elaine Rich and Kevin Knight.
;;      This implements the traditional Missionaries and Cannabals
;;      problem.
;;
;; ------------------------------------------------------------------

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

 (goal-state? s)                t if s is a goal state
 (eq-states s1 s2)              are s1 and s2 equal?
 (expand s)                     successor states of s
 (print-state s)                print function
 (heuristic s)                  heuristic distance between s and the goal
 (generate-problem)             randomly generated start-state
 (cost-of-move s1 s2)           cost of moving from s1 to s2

The important variables are:

 *sample-initial-state*
 *goal-state*

These functions and variables can all be called from an outside search
program.  In fact, these are the functions called by our implementations of
depth-first, breadth-first, hill-climbing, A*, DFID, IDA*, and RTA* search.

For example domains, see "n-puzzle.lisp" and "travel.lisp".

--------------------------------------------------------------------------|#

;; ------------------------------------------------------------------
;;                 Variables
;; ------------------------------------------------------------------

;; Variable *INFINITY* will be used as the largest possible number.
;; MOST-POSITIVE-FIXNUM is a Lisp symbol that provides it.
(defvar *infinity* most-positive-fixnum)

;; Variable *SAMPLE-INITIAL-STATE* holds a sample initial state.
(defvar *sample-initial-state* '(3 3 L))

;; Variable *GOAL-STATE* holds a goal state.  This can be nil if there
;; is no unique goal state.
(defvar *goal-state* '(0 0 R))

;; ------------------------------------------------------------------
;;  This line will load the BFS.LSP file, which is needed for this
;;  program to operate correctly.
(load "bfs.lsp")


;; ------------------------------------------------------------------
;; Functions required by search programs in any domain.
;; ------------------------------------------------------------------

;; ------------------------------------------------------------------
;;                 GOAL-STATE?
;; ------------------------------------------------------------------
;; SYNOPSIS:     Tests if the state s is the goal state.
;; CALLED BY:    BFS.LSP
;; PARAMETERS:   (s) List representing the current state of problem.
;; RETURNS:      t if s is a goal state, nil if not
;; SIDE EFFECTS:
;; STRATEGY:     Use the eql LISP function to check if s is equal
;;               to the *goal-state* constant.
;; ------------------------------------------------------------------
(defun goal-state? (s)
  (equal s *goal-state*))

;; ------------------------------------------------------------------
;;                 EQ-STATES
;; ------------------------------------------------------------------
;; SYNOPSIS:     Tests if the two states are the same.
;; CALLED BY:    BFS.LSP
;; PARAMETERS:   (s1 s2) Two states to compare
;; RETURNS:      t if s1 and s2 equal, nil if not
;; SIDE EFFECTS:
;; STRATEGY:     Use the eql LISP function to check if s1 is equal
;;               to s2.
;; ------------------------------------------------------------------
(defun eq-states (s1 s2)
  (equal s1 s2))

;; ------------------------------------------------------------------
;;                 CHECK-MOVE
;; ------------------------------------------------------------------
;; SYNOPSIS:     Checks to see if move is possible under our
;;               rule set.
;; CALLED BY:    EXPAND in MISSCAN.LSP
;; PARAMETERS:   (l) List of form (Ma, Ca, Mb, Cb, m, c)
;;               Where Ma is missionaries from and Mb is missionaries
;;               to; m is number missionaries moving.
;; RETURNS:      t if move possible, nil otherwise
;; SIDE EFFECTS: 
;; STRATEGY:
;; ------------------------------------------------------------------
(defun check-move (l)
   (if (and (and (>= (first l) (fifth l)) 
		 (>= (second l) (sixth l)))
	    (and (or (>= (+ (third l) (fifth l)) (+ (fourth l) (sixth l)))
		     (= (+ (third l) (fifth l)) 0))
		 (or (>= (- (first l) (fifth l)) (- (second l) (sixth l)))
		     (= (- (first l) (fifth l)) 0))))
    t))

;; ------------------------------------------------------------------
;;                 CHECK-LEGAL
;; ------------------------------------------------------------------
;; SYNOPSIS:     Checks to see if move is possible under our
;;               rule set.
;; CALLED BY:    EXPAND in MISSCAN.LSP
;; PARAMETERS:   (l) List of form (Ml, Cl, Mr, Cr )
;;               Where Ml is missionaries on the left and Mr is 
;;               missionaries on the right.
;; RETURNS:      t if move possible, nil otherwise
;; SIDE EFFECTS: 
;; STRATEGY:
;; ------------------------------------------------------------------
(defun check-legal (l)
  (if (and (>= (first l) (second l))
	   (>= (third l) (fourth l))
	   (= (+ (first l) (third l)) 3)
	   (= (+ (second l) (fourth l)) 3))
    t))

;; ------------------------------------------------------------------
;;                 EXPAND
;; ------------------------------------------------------------------
;; SYNOPSIS:     Create a list of children from the state s.
;; CALLED BY:    BFS.LSP
;; PARAMETERS:   (s) The state to expand.
;; RETURNS:      List of successors to s.
;; SIDE EFFECTS:
;; STRATEGY:     
;; ------------------------------------------------------------------
(defun expand (s)
  (let ((succs nil)
	(children nil))
    (when (eql (third s) 'L)
	(setq succs (append succs (list (list (- (first s) 2)
					      (second s) 'R))))
	(setq succs (append succs (list (list (- (first s) 1)
					      (second s) 'R))))
	(setq succs (append succs (list (list (- (first s) 1)
					      (- (second s) 1) 'R))))
	(setq succs (append succs (list (list (first s)
					(- (second s) 2) 'R))))
	(setq succs (append succs (list (list (first s)
					      (- (second s) 1) 'R)))))
    (when (eql (third s) 'R)
	(setq succs (append succs (list (list (+ (first s) 2)
					      (second s) 'L))))
	(setq succs (append succs (list (list (+ (first s) 1)
					      (second s) 'L))))
	(setq succs (append succs (list (list (+ (first s) 1)
					      (+ (second s) 1) 'L))))
	(setq succs (append succs (list (list (first s)
					      (+ (second s) 2) 'L))))
	(setq succs (append succs (list (list (first s)
					      (+ (second s) 1) 'L)))))
    (dolist (count succs)
      (if (check-legal (list (first (nth count succs))
			     (second (nth count succs))
			     (- 3 (first (nth count succs)))
			     (- 3 (second (nth count succs)))))
	(setq children (append children (list (nth count succs))))))
    children))
      
;; ------------------------------------------------------------------
;;                 PRINT-STATE
;; ------------------------------------------------------------------
;; SYNOPSIS:     Prints the state to the screen.
;; CALLED BY:    BFS.LSP
;; PARAMETERS:   (s) State to print.
;; RETURNS:      nil
;; SIDE EFFECTS:
;; STRATEGY:     Use print function to display the state.
;; ------------------------------------------------------------------
(defun print-state (s &rest ignore)
   (declare (ignore ignore))
   (print s))

;; ------------------------------------------------------------------
;;                 HEURISTIC
;; ------------------------------------------------------------------
;; SYNOPSIS:     Provides a numerical estimate of the difficulty of 
;;               reaching a goal state from s.  The higher the 
;;               estimate, the more difficulty.
;; CALLED BY:    BFS.LSP
;; PARAMETERS:   (s) List representing the state to examine.
;; RETURNS:      nil
;; SIDE EFFECTS:
;; STRATEGY:     Return nil
;; ------------------------------------------------------------------
(defun heuristic (s)
  nil)

;; ------------------------------------------------------------------
;;                 GENERATE-PROBLEM
;; ------------------------------------------------------------------
;; SYNOPSIS:     Returns the starting state of the problem.
;; CALLED BY:    BFS.LSP
;; PARAMETERS:
;; RETURNS:      state - Represents the start state of the problem.
;; SIDE EFFECTS:
;; STRATEGY:     Return the start state of (3 3 L).
;; ------------------------------------------------------------------
(defun generate-problem ()
  *sample-initial-state*)

;; ------------------------------------------------------------------
;;                 COST-OF-MOVE
;; ------------------------------------------------------------------
;; SYNOPSIS:     Returns the cost of moving from the current state
;;               to the goal state.
;; CALLED BY:    BFS.LSP
;; PARAMETERS:   (s) State to calculate cost of.
;; RETURNS:      1
;; SIDE EFFECTS:
;; STRATEGY:     Return the value 1, since we're doing a BFS in this
;;               assignment.
;; ------------------------------------------------------------------
(defun cost-of-move (s)
  1)
