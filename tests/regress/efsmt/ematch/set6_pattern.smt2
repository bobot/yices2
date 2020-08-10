(set-info :smt-lib-version 2.6)
(set-logic UF)
(set-info :source | Set theory. |)
(set-info :category "crafted")
(set-info :status unsat)
(declare-sort Set 0)
(declare-sort Elem 0)
(declare-fun member (Elem Set) Bool)
(declare-fun subset (Set Set) Bool)

(assert (forall ((?s1 Set) (?s2 Set)) (! (=> (not (subset ?s1 ?s2)) (exists ((?x Elem)) (and (member ?x ?s1) (not (member ?x ?s2)))))
													:pattern ((subset ?s1 ?s2)) )))

(declare-fun seteq (Set Set) Bool)
(assert (forall ((?s1 Set) (?s2 Set)) (! (= (seteq ?s1 ?s2) (and (subset ?s1 ?s2) (subset ?s2 ?s1)))
										:pattern ((seteq ?s1 ?s2)) )))

(declare-fun union (Set Set) Set)
(assert (forall ((?x Elem) (?s1 Set) (?s2 Set)) (! (= (member ?x (union ?s1 ?s2)) (or (member ?x ?s1) (member ?x ?s2)))
										:pattern ((member ?x (union ?s1 ?s2))) )))

(declare-fun intersection (Set Set) Set)
(assert (forall ((?x Elem) (?s1 Set) (?s2 Set)) (! (= (member ?x (intersection ?s1 ?s2)) (and (member ?x ?s1) (member ?x ?s2)))
										:pattern ((member ?x (intersection ?s1 ?s2))) )))

(declare-fun difference (Set Set) Set)
(assert (forall ((?x Elem) (?s1 Set) (?s2 Set)) (! (= (member ?x (difference ?s1 ?s2)) (and (member ?x ?s1) (not (member ?x ?s2))))
										:pattern ((member ?x (difference ?s1 ?s2))) )))

(declare-fun a () Set)
(declare-fun b () Set)
(declare-fun c () Set)
(assert (not (seteq (union (difference c a) (difference c b)) (difference c (intersection a b)))))

(check-sat)
(exit)
