(set-logic QF_BV)
(declare-const v0 Bool)
(declare-const v1 Bool)
(declare-const v2 Bool)
(declare-const v3 Bool)
(declare-const v4 Bool)
(declare-const v5 Bool)
(declare-const v6 Bool)
(declare-const v7 Bool)
(declare-const v8 Bool)
(declare-const v9 Bool)
(declare-const v10 Bool)
(declare-const v11 Bool)
(declare-const v12 Bool)
(declare-const v13 Bool)
(declare-const v14 Bool)
(declare-const v15 Bool)
(declare-const v16 Bool)
(declare-const v17 Bool)
(check-sat)
(assert v6)
(declare-const v18 Bool)
(assert (= (or v11 v16 v0 v9 v1 v2 v4 v11) v13 v16 v5 v7 (or v11 v16 v0 v9 v1 v2 v4 v11) v16 v8 v1))
(declare-const v19 Bool)
(declare-const v20 Bool)
(assert v7)
(declare-const v21 Bool)
(assert v10)
(declare-const bv_5-0 (_ BitVec 5))
(declare-const v22 Bool)
(assert v15)
(declare-const v23 Bool)
(assert v21)
(declare-const v24 Bool)
(declare-const bv_8-2 (_ BitVec 8))
(assert (and (= v13 v18 v10) v0 v21 v24))
(assert (and v3 v16 v22 v10 v18 v24))
(assert v0)
(declare-const v25 Bool)
(push 1)
(assert (xor v20 v23 v11 v9 (= v13 v18 v10) (and v3 v16 v22 v10 v18 v24)))
(declare-const bv_1-0 (_ BitVec 1))
(declare-const v26 Bool)
(assert v0)
(declare-const bv_17-0 (_ BitVec 17))
(assert (= (distinct bv_1-0 bv_1-0 bv_1-0) v17 v15 v10 v6 (xor v20 v23 v11 v9 (= v13 v18 v10) (and v3 v16 v22 v10 v18 v24)) (= ((_ rotate_left 2) #b01010000111) (bvnand ((_ rotate_left 2) #b01010000111) ((_ rotate_left 2) #b01010000111)) ((_ rotate_left 2) #b01010000111) ((_ rotate_left 2) #b01010000111) (bvnand ((_ rotate_left 2) #b01010000111) ((_ rotate_left 2) #b01010000111))) (or v11 v16 v0 v9 v1 v2 v4 v11) v22))
(declare-const v27 Bool)
(assert v10)
(declare-const v28 Bool)
(assert (= (= ((_ rotate_left 2) #b01010000111) (bvnand ((_ rotate_left 2) #b01010000111) ((_ rotate_left 2) #b01010000111)) ((_ rotate_left 2) #b01010000111) ((_ rotate_left 2) #b01010000111) (bvnand ((_ rotate_left 2) #b01010000111) ((_ rotate_left 2) #b01010000111))) v23 (or v11 v16 v0 v9 v1 v2 v4 v11)))
(declare-const v29 Bool)
(declare-const v30 Bool)
(assert v16)
(pop 1)
(declare-const v31 Bool)
(assert (xor v8 v12 v11 (or v11 v16 v0 v9 v1 v2 v4 v11) v9))
(assert (or (= (bvmul bv_5-0 bv_5-0) bv_5-0 (bvmul bv_5-0 bv_5-0) bv_5-0 bv_5-0) v20 v7 v21 v13 v0 v6 (or (and v3 v16 v22 v10 v18 v24) v9 v23 v19 (= (or v11 v16 v0 v9 v1 v2 v4 v11) v13 v16 v5 v7 (or v11 v16 v0 v9 v1 v2 v4 v11) v16 v8 v1) v6 v20) v5 v24 v31))
(assert (bvsle (bvxnor bv_8-2 bv_8-2) bv_8-2))
(assert v0)
(declare-const v32 Bool)
(declare-const bv_8-4 (_ BitVec 8))
(declare-const v33 Bool)
(assert (xor v18 v22 v25 v2 v4 v8 v3))
(assert (xor (=> v11 (or v14 (= (or v11 v16 v0 v9 v1 v2 v4 v11) v13 v16 v5 v7 (or v11 v16 v0 v9 v1 v2 v4 v11) v16 v8 v1) (xor v8 v12 v11 (or v11 v16 v0 v9 v1 v2 v4 v11) v9))) v21 (and (bvsle ((_ extract 3 1) bv_5-0) ((_ extract 3 1) bv_5-0)) v15) v5 v10 v18 (or v11 v16 v0 v9 v1 v2 v4 v11) v25 v7 (or v2 v15 v20 v2 (xor v8 v12 v11 (or v11 v16 v0 v9 v1 v2 v4 v11) v9) v0 (and (= v13 v18 v10) v0 v21 v24) (distinct bv_5-0 bv_5-0))))
(assert (= (bvmul bv_5-0 bv_5-0) bv_5-0 (bvmul bv_5-0 bv_5-0) bv_5-0 bv_5-0))
(assert (or (and v3 v16 v22 v10 v18 v24) v9 v23 v19 (= (or v11 v16 v0 v9 v1 v2 v4 v11) v13 v16 v5 v7 (or v11 v16 v0 v9 v1 v2 v4 v11) v16 v8 v1) v6 v20))
(push 1)
(declare-const v34 Bool)
(assert (xor v25 v2 v6 (bvsle ((_ extract 3 1) bv_5-0) ((_ extract 3 1) bv_5-0)) v14 v10 (=> v11 (= (or v11 v16 v0 v9 v1 v2 v4 v11) v13 v16 v5 v7 (or v11 v16 v0 v9 v1 v2 v4 v11) v16 v8 v1))))
(declare-const v35 Bool)
(assert v21)
(assert (and (= v13 v18 v10) v0 v21 v24))
(push 1)
(push 1)
(declare-const bv_9-0 (_ BitVec 9))
(declare-const v36 Bool)
(assert v5)
(declare-const bv_14-0 (_ BitVec 14))
(assert (and v33 v18 v18 v20 v19 (and v3 v16 v22 v10 v18 v24) (distinct bv_5-0 bv_5-0) (= (or v11 v16 v0 v9 v1 v2 v4 v11) v13 v16 v5 v7 (or v11 v16 v0 v9 v1 v2 v4 v11) v16 v8 v1) v25 (and v3 v16 v22 v10 v18 v24)))
(push 1)
(declare-const v37 Bool)
(assert (not v5))
(declare-const bv_21-0 (_ BitVec 21))
(assert v10)
(declare-const v38 Bool)
(assert v18)
(push 1)
(assert (or v25 (and (bvsle ((_ extract 3 1) bv_5-0) ((_ extract 3 1) bv_5-0)) v15) (not v5) (or (and v3 v16 v22 v10 v18 v24) v9 v23 v19 (= (or v11 v16 v0 v9 v1 v2 v4 v11) v13 v16 v5 v7 (or v11 v16 v0 v9 v1 v2 v4 v11) v16 v8 v1) v6 v20) v18 (and (bvsle ((_ extract 3 1) bv_5-0) ((_ extract 3 1) bv_5-0)) v15) (and v3 v16 v22 v10 v18 v24) (or v14 (= (or v11 v16 v0 v9 v1 v2 v4 v11) v13 v16 v5 v7 (or v11 v16 v0 v9 v1 v2 v4 v11) v16 v8 v1) (xor v8 v12 v11 (or v11 v16 v0 v9 v1 v2 v4 v11) v9)) (=> v11 (or v14 (= (or v11 v16 v0 v9 v1 v2 v4 v11) v13 v16 v5 v7 (or v11 v16 v0 v9 v1 v2 v4 v11) v16 v8 v1) (xor v8 v12 v11 (or v11 v16 v0 v9 v1 v2 v4 v11) v9))) v35 (and (bvsle ((_ extract 3 1) bv_5-0) ((_ extract 3 1) bv_5-0)) v15)))
(declare-const v39 Bool)
(assert (or v2 v15 v20 v2 (xor v8 v12 v11 (or v11 v16 v0 v9 v1 v2 v4 v11) v9) v0 (and (= v13 v18 v10) v0 v21 v24) (distinct bv_5-0 bv_5-0)))
(declare-const bv_30-0 (_ BitVec 30))
(assert (or (= (bvmul bv_5-0 bv_5-0) bv_5-0 (bvmul bv_5-0 bv_5-0) bv_5-0 bv_5-0) v20 v7 v21 v13 v0 v6 (or (and v3 v16 v22 v10 v18 v24) v9 v23 v19 (= (or v11 v16 v0 v9 v1 v2 v4 v11) v13 v16 v5 v7 (or v11 v16 v0 v9 v1 v2 v4 v11) v16 v8 v1) v6 v20) v5 v24 v31))
(assert (or v25 (and (bvsle ((_ extract 3 1) bv_5-0) ((_ extract 3 1) bv_5-0)) v15) (not v5) (or (and v3 v16 v22 v10 v18 v24) v9 v23 v19 (= (or v11 v16 v0 v9 v1 v2 v4 v11) v13 v16 v5 v7 (or v11 v16 v0 v9 v1 v2 v4 v11) v16 v8 v1) v6 v20) v18 (and (bvsle ((_ extract 3 1) bv_5-0) ((_ extract 3 1) bv_5-0)) v15) (and v3 v16 v22 v10 v18 v24) (or v14 (= (or v11 v16 v0 v9 v1 v2 v4 v11) v13 v16 v5 v7 (or v11 v16 v0 v9 v1 v2 v4 v11) v16 v8 v1) (xor v8 v12 v11 (or v11 v16 v0 v9 v1 v2 v4 v11) v9)) (=> v11 (or v14 (= (or v11 v16 v0 v9 v1 v2 v4 v11) v13 v16 v5 v7 (or v11 v16 v0 v9 v1 v2 v4 v11) v16 v8 v1) (xor v8 v12 v11 (or v11 v16 v0 v9 v1 v2 v4 v11) v9))) v35 (and (bvsle ((_ extract 3 1) bv_5-0) ((_ extract 3 1) bv_5-0)) v15)))
(declare-const v40 Bool)
(assert v33)
(declare-const v41 Bool)
(declare-const v42 Bool)
(assert (=> (and (bvsle (bvxnor bv_8-2 bv_8-2) bv_8-2) (= (bvxor (bvmul bv_5-0 bv_5-0) bv_5-0) (bvmul bv_5-0 bv_5-0) (bvxor (bvmul bv_5-0 bv_5-0) bv_5-0) (bvmul bv_5-0 bv_5-0)) (or v11 v16 v0 v9 v1 v2 v4 v11) (or v11 v16 v0 v9 v1 v2 v4 v11) v0 v24) v6))
(declare-const v43 Bool)
(declare-const bv_10-8 (_ BitVec 10))
(assert (not v33))
(check-sat)
(declare-const v44 Bool)
(assert (= v24 (or v14 (= (or v11 v16 v0 v9 v1 v2 v4 v11) v13 v16 v5 v7 (or v11 v16 v0 v9 v1 v2 v4 v11) v16 v8 v1) (xor v8 v12 v11 (or v11 v16 v0 v9 v1 v2 v4 v11) v9)) (distinct bv_30-0 bv_30-0 bv_30-0 bv_30-0 bv_30-0) v18 (and v33 v18 v18 v20 v19 (and v3 v16 v22 v10 v18 v24) (distinct bv_5-0 bv_5-0) (= (or v11 v16 v0 v9 v1 v2 v4 v11) v13 v16 v5 v7 (or v11 v16 v0 v9 v1 v2 v4 v11) v16 v8 v1) v25 (and v3 v16 v22 v10 v18 v24)) (not v5) v2 v0 (and (bvsle (bvxnor bv_8-2 bv_8-2) bv_8-2) (= (bvxor (bvmul bv_5-0 bv_5-0) bv_5-0) (bvmul bv_5-0 bv_5-0) (bvxor (bvmul bv_5-0 bv_5-0) bv_5-0) (bvmul bv_5-0 bv_5-0)) (or v11 v16 v0 v9 v1 v2 v4 v11) (or v11 v16 v0 v9 v1 v2 v4 v11) v0 v24) (xor (=> v11 (or v14 (= (or v11 v16 v0 v9 v1 v2 v4 v11) v13 v16 v5 v7 (or v11 v16 v0 v9 v1 v2 v4 v11) v16 v8 v1) (xor v8 v12 v11 (or v11 v16 v0 v9 v1 v2 v4 v11) v9))) v21 (and (bvsle ((_ extract 3 1) bv_5-0) ((_ extract 3 1) bv_5-0)) v15) v5 v10 v18 (or v11 v16 v0 v9 v1 v2 v4 v11) v25 v7 (or v2 v15 v20 v2 (xor v8 v12 v11 (or v11 v16 v0 v9 v1 v2 v4 v11) v9) v0 (and (= v13 v18 v10) v0 v21 v24) (distinct bv_5-0 bv_5-0))) v7))
(check-sat)
(declare-const v45 Bool)
(check-sat)
(declare-const bv_13-0 (_ BitVec 13))
(assert (distinct bv_5-0 bv_5-0))
(declare-const v46 Bool)
(assert (not (xor v8 v12 v11 (or v11 v16 v0 v9 v1 v2 v4 v11) v9)))
(declare-const v47 Bool)
(assert v25)
(pop 1)
(pop 1)
(assert (xor v15 v14 v31 v35 v22 (xor v8 v12 v11 (or v11 v16 v0 v9 v1 v2 v4 v11) v9) v8))
(pop 1)
(declare-const v48 Bool)
(check-sat)
(declare-const v49 Bool)
(declare-const v50 Bool)
(assert (not (and (= v13 v18 v10) v0 v21 v24)))
(declare-const v51 Bool)
(assert (or v1 (=> v11 (= (or v11 v16 v0 v9 v1 v2 v4 v11) v13 v16 v5 v7 (or v11 v16 v0 v9 v1 v2 v4 v11) v16 v8 v1)) (=> v11 (= (or v11 v16 v0 v9 v1 v2 v4 v11) v13 v16 v5 v7 (or v11 v16 v0 v9 v1 v2 v4 v11) v16 v8 v1)) v32 v12 v22 v9 v49 v2 (or v14 (= (or v11 v16 v0 v9 v1 v2 v4 v11) v13 v16 v5 v7 (or v11 v16 v0 v9 v1 v2 v4 v11) v16 v8 v1) (xor v8 v12 v11 (or v11 v16 v0 v9 v1 v2 v4 v11) v9))))
(declare-const v52 Bool)
(assert (or (or v2 v15 v20 v2 (xor v8 v12 v11 (or v11 v16 v0 v9 v1 v2 v4 v11) v9) v0 (and (= v13 v18 v10) v0 v21 v24) (distinct bv_5-0 bv_5-0)) v8 v52))
(declare-const bv_27-0 (_ BitVec 27))
(assert (xor v3 v7 (not (and (= v13 v18 v10) v0 v21 v24)) (=> (xor v8 v12 v11 (or v11 v16 v0 v9 v1 v2 v4 v11) v9) v35) v34 v7 (or v11 v16 v0 v9 v1 v2 v4 v11) v49 v31 v13 (=> v11 (= (or v11 v16 v0 v9 v1 v2 v4 v11) v13 v16 v5 v7 (or v11 v16 v0 v9 v1 v2 v4 v11) v16 v8 v1))))
(assert v2)
(check-sat)
(declare-const v53 Bool)
(push 1)
(assert (=> (bvsle (bvxnor bv_8-2 bv_8-2) bv_8-2) (xor v3 v7 (not (and (= v13 v18 v10) v0 v21 v24)) (=> (xor v8 v12 v11 (or v11 v16 v0 v9 v1 v2 v4 v11) v9) v35) v34 v7 (or v11 v16 v0 v9 v1 v2 v4 v11) v49 v31 v13 (=> v11 (= (or v11 v16 v0 v9 v1 v2 v4 v11) v13 v16 v5 v7 (or v11 v16 v0 v9 v1 v2 v4 v11) v16 v8 v1)))))
(assert (or v6 v50 v8 v35 (= (bvudiv bv_8-2 (bvsmod #b10011000 #b10011000)) (bvudiv (bvudiv bv_8-2 (bvsmod #b10011000 #b10011000)) (bvudiv bv_8-2 (bvsmod #b10011000 #b10011000))) #b10011000 (bvudiv bv_8-2 (bvsmod #b10011000 #b10011000))) (=> (bvsle (bvxnor bv_8-2 bv_8-2) bv_8-2) (xor v3 v7 (not (and (= v13 v18 v10) v0 v21 v24)) (=> (xor v8 v12 v11 (or v11 v16 v0 v9 v1 v2 v4 v11) v9) v35) v34 v7 (or v11 v16 v0 v9 v1 v2 v4 v11) v49 v31 v13 (=> v11 (= (or v11 v16 v0 v9 v1 v2 v4 v11) v13 v16 v5 v7 (or v11 v16 v0 v9 v1 v2 v4 v11) v16 v8 v1)))) v10 (distinct bv_5-0 bv_5-0) (and (bvsle (bvxnor bv_8-2 bv_8-2) bv_8-2) (= (bvxor (bvmul bv_5-0 bv_5-0) bv_5-0) (bvmul bv_5-0 bv_5-0) (bvxor (bvmul bv_5-0 bv_5-0) bv_5-0) (bvmul bv_5-0 bv_5-0)) (or v11 v16 v0 v9 v1 v2 v4 v11) (or v11 v16 v0 v9 v1 v2 v4 v11) v0 v24) v24 (= (bvmul bv_5-0 bv_5-0) bv_5-0 (bvmul bv_5-0 bv_5-0) bv_5-0 bv_5-0)))
(push 1)
(declare-const v54 Bool)
(assert v31)
(declare-const v55 Bool)
(check-sat)
(declare-const v56 Bool)
(pop 1)
(declare-const v57 Bool)
(declare-const v58 Bool)
(pop 1)
(assert v32)
(assert (and (= v13 v18 v10) v0 v21 v24))
(assert v17)
(assert (xor v8 v12 v11 (or v11 v16 v0 v9 v1 v2 v4 v11) v9))
(assert (bvsle ((_ extract 3 1) bv_5-0) ((_ extract 3 1) bv_5-0)))
(pop 1)
(declare-const v59 Bool)
(declare-const bv_18-0 (_ BitVec 18))
(declare-const v60 Bool)
(assert (xor v34 (or v2 v15 v20 v2 (xor v8 v12 v11 (or v11 v16 v0 v9 v1 v2 v4 v11) v9) v0 (and (= v13 v18 v10) v0 v21 v24) (distinct bv_5-0 bv_5-0)) v4 v6 (bvsle ((_ extract 3 1) bv_5-0) ((_ extract 3 1) bv_5-0)) v60 v15 (xor (=> v11 (or v14 (= (or v11 v16 v0 v9 v1 v2 v4 v11) v13 v16 v5 v7 (or v11 v16 v0 v9 v1 v2 v4 v11) v16 v8 v1) (xor v8 v12 v11 (or v11 v16 v0 v9 v1 v2 v4 v11) v9))) v21 (and (bvsle ((_ extract 3 1) bv_5-0) ((_ extract 3 1) bv_5-0)) v15) v5 v10 v18 (or v11 v16 v0 v9 v1 v2 v4 v11) v25 v7 (or v2 v15 v20 v2 (xor v8 v12 v11 (or v11 v16 v0 v9 v1 v2 v4 v11) v9) v0 (and (= v13 v18 v10) v0 v21 v24) (distinct bv_5-0 bv_5-0)))))
(declare-const v61 Bool)
(assert (and (= v13 v18 v10) v0 v21 v24))
(check-sat)
