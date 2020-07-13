/*
 * This file is part of the Yices SMT Solver.
 * Copyright (C) 2017 SRI International.
 *
 * Yices is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Yices is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Yices.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * SOLVER FOR QUANTIFIERS
 */



#include <inttypes.h>

#include "io/tracer.h"
#include "solvers/quant/quant_solver.h"
#include "solvers/quant/quant_ematching.h"
#include "utils/hash_functions.h"
#include "utils/index_vectors.h"
#include "utils/int_array_sort2.h"
#include "utils/int_hash_classes.h"
#include "utils/memalloc.h"
#include "utils/pointer_vectors.h"
#include "utils/ptr_array_sort2.h"
#include "utils/ptr_partitions.h"
#include "terms/term_explorer.h"
#include "yices.h"
#include "context/internalization_codes.h"
#include "context/quant_context_utils.h"
#include "terms/term_substitution.h"

#define EM_VERBOSE 0

#define TRACE 0

#if TRACE

#include <stdio.h>

#include "solvers/cdcl/smt_core_printer.h"
#include "solvers/egraph/egraph_printer.h"

#endif


/**********************
 *  PRINTING SUPPORT  *
 *********************/

static void quant_solver_print_pattern(FILE *f, quant_solver_t *solver, uint32_t i) {
  pattern_t *pat;
  uint32_t n;

  assert(i < solver->ptbl.npatterns);

  pat = solver->ptbl.data + i;

  fprintf(f, "    pattern @%d: ", i);
  yices_pp_term(f, pat->p, 120, 1, 0);

  n = iv_len(pat->pvars);
  fprintf(f, "      pvars (#%d): ", n);
  yices_pp_term_array(f, n, pat->pvars, 120, 1, 0, 1);

  n = iv_len(pat->fun);
  fprintf(f, "      fun (#%d): ", n);
  yices_pp_term_array(f, n, pat->fun, 120, 1, 0, 1);

  n = iv_len(pat->fapps);
  fprintf(f, "      fapps (#%d): ", n);
  yices_pp_term_array(f, n, pat->fapps, 120, 1, 0, 1);

  n = iv_len(pat->consts);
  fprintf(f, "      consts (#%d): ", n);
  yices_pp_term_array(f, n, pat->consts, 120, 1, 0, 1);

  fprintf(f, "\n");

}

static void quant_solver_print_cnstr(FILE *f, quant_solver_t *solver, uint32_t i) {
  quant_cnstr_t *qcnstr;
  uint32_t j, n;

  assert(i < solver->qtbl.nquant);

  qcnstr = solver->qtbl.data + i;
  fprintf(f, "\nqcnstr[%d]:\n", i);

  fprintf(f, "  expr: ");
  yices_pp_term(f, qcnstr->t, 120, 1, 0);

  n = iv_len(qcnstr->uvars);
  fprintf(f, "      uvars (#%d): ", n);
  yices_pp_term_array(f, n, qcnstr->uvars, 120, 1, 0, 1);

  n = iv_len(qcnstr->fun);
  fprintf(f, "      fun (#%d): ", n);
  yices_pp_term_array(f, n, qcnstr->fun, 120, 1, 0, 1);

  n = iv_len(qcnstr->fapps);
  fprintf(f, "      fapps (#%d): ", n);
  yices_pp_term_array(f, n, qcnstr->fapps, 120, 1, 0, 1);

  n = iv_len(qcnstr->consts);
  fprintf(f, "      consts (#%d): ", n);
  yices_pp_term_array(f, n, qcnstr->consts, 120, 1, 0, 1);

  n = iv_len(qcnstr->patterns);
  fprintf(f, "  patterns (#%d):\n", n);
  for (j=0; j<n; j++)
    quant_solver_print_pattern(f, solver, qcnstr->patterns[j]);

  fprintf(f, "\n");

}


/**************************
 *  PROBLEM CONSTRUCTION  *
 *************************/

/*
 * Infer patterns for term t
 *   infer new patterns and add them to patterns vector
 */
static void quant_infer_patterns(quant_solver_t *solver, term_t t, ivector_t *patterns, ivector_t *uvars) {
  ivector_t prospectives;
  term_t x;
  uint32_t i, n;

#if TRACE
  printf("  Inferring pattern for ");
  yices_pp_term(stdout, t, 120, 1, 0);
#endif

  init_ivector(&prospectives, 4);

  quant_infer_single_pattern(solver->prob->terms, t, uvars, &prospectives);

  n = prospectives.size;
  if (n != 0) {
#if TRACE
    printf("    found #%d prospectives: ", n);
    yices_pp_term_array(stdout, n, prospectives.data, 120, 1, 0, 1);
#endif

//    n = 1;    // restrict to only the first inferred pattern
    for(i=0; i<n; i++) {
      x = prospectives.data[i];
      ivector_push(patterns, x);
    }

#if TRACE
    printf("    added #%d patterns\n", n);
#endif
  }

  delete_ivector(&prospectives);
}


/*
 * Preprocess pattern, add pattern to pattern table, and return its index
 */
static int32_t quant_preprocess_pattern(quant_solver_t *solver, term_t t, term_t pat) {
  int_hmap_pair_t *r;
  r = int_hmap_get(&solver->aux_map, pat);
  if (r->val >= 0) {
    return r->val;
  }

  ivector_t pv, f, fa, c;
  int32_t i;

  init_ivector(&pv, 5);
  init_ivector(&f, 5);
  init_ivector(&fa, 5);
  init_ivector(&c, 5);

  quant_process_pattern_term(solver->prob->terms, pat, &pv, &f, &fa, &c);
  ivector_remove_duplicates(&pv);
  ivector_remove_duplicates(&f);
  ivector_remove_duplicates(&fa);
  ivector_remove_duplicates(&c);

  i = pattern_table_add_pattern(&solver->ptbl, pat, pv.data, pv.size, f.data, f.size,
                           fa.data, fa.size, c.data, c.size);

  delete_ivector(&pv);
  delete_ivector(&f);
  delete_ivector(&fa);
  delete_ivector(&c);

#if 0
  printf("    Adding pattern:\n");
  quant_solver_print_pattern(stdout, solver, i);
#endif

  r->val = i;
  return i;
}

/*
 * Setup patterns for term t
 *   if patterns present, try pruning
 *   else, infer patterns
 *   add all the final patterns and push their indices in out vector
 */
static void quant_setup_patterns(quant_solver_t *solver, term_t t, ivector_t *patterns, ivector_t *uvars, ivector_t *out) {
  uint32_t i, n;
  term_t *data;
  term_t x;
  int32_t idx;

  n = patterns->size;
  if (n == 0) {
    quant_infer_patterns(solver, t, patterns, uvars);
    n = patterns->size;
  }
#if 0
  printf("  Total #%d patterns\n", n);
#endif

  data = patterns->data;
  for (i=0; i<n; i++) {
    x = data[i];

    idx = quant_preprocess_pattern(solver, t, x);

    assert(idx >= 0);
    ivector_push(out, idx);
    assert(x == solver->ptbl.data[idx].p);
  }
}

/*
 * Preprocess problem constraint
 */
static int32_t quant_preprocess_assertion_with_pattern(quant_solver_t *solver, term_t t, ivector_t *patterns) {
  quant_table_t *qtbl;
  int32_t i;
  ivector_t v;
  quant_cnstr_t *cnstr;
  ivector_t pv, f, fa, c;
  bool valid;

  qtbl = &solver->qtbl;
  init_ivector(&v, 1);
  init_ivector(&pv, 5);
  init_ivector(&f, 5);
  init_ivector(&fa, 5);
  init_ivector(&c, 5);

  quant_process_pattern_term(solver->prob->terms, t, &pv, &f, &fa, &c);
  ivector_remove_duplicates(&pv);
  ivector_remove_duplicates(&f);
  ivector_remove_duplicates(&fa);
  ivector_remove_duplicates(&c);

  quant_setup_patterns(solver, t, patterns, &pv, &v);

  i = quant_table_add_cnstr(qtbl, t, v.data, v.size);
  cnstr = qtbl->data + i;

  cnstr->uvars = make_index_vector(pv.data, pv.size);
  cnstr->fun = make_index_vector(f.data, f.size);
  cnstr->fapps = make_index_vector(fa.data, fa.size);
  cnstr->consts = make_index_vector(c.data, c.size);

  valid = quant_table_check_cnstr(qtbl, &solver->ptbl, i);
  if (!valid) {
    printf("\nError in assertion + pattern for:\n");
    quant_solver_print_cnstr(stdout, solver, i);
    assert(0);
  }

  delete_ivector(&v);
  delete_ivector(&pv);
  delete_ivector(&f);
  delete_ivector(&fa);
  delete_ivector(&c);

#if TRACE
  quant_solver_print_cnstr(stdout, solver, i);
#endif

  return i;
}

/*
 * Preprocess problem
 */
static void quant_preprocess_prob(quant_solver_t *solver) {
  ef_prob_t *prob;
  ptr_hmap_t *patterns;
  ptr_hmap_pair_t *r;

  prob = solver->prob;
  patterns = prob->patterns;
  if (patterns != NULL) {
    for (r = ptr_hmap_first_record(patterns);
         r != NULL;
         r = ptr_hmap_next_record(patterns, r)) {
      quant_preprocess_assertion_with_pattern(solver, r->key, r->val);
    }
  }
}

/*
 * Attach problem to solver
 */
void quant_solver_attach_prob(quant_solver_t *solver, ef_prob_t *prob, context_t *ctx) {
  assert(solver->prob == NULL);

  solver->prob = prob;
  quant_preprocess_prob(solver);
//  assert(0);

  ematch_attach_tbl(&solver->em, solver->prob->terms, &solver->ptbl, &solver->qtbl, ctx);
  ematch_compile_all_patterns(&solver->em);

  ematch_attach_egraph(&solver->em, solver->egraph);

}


/***********************
 *  EMATCHING SUPPORT  *
 **********************/

/*
 * Apply the substitution defined by var and value to term t
 * - n = size of arrays var and value
 * - return code < 0 means that an error occurred during the substitution
 *   (cf. apply_term_subst in term_substitution.h).
 */
static term_t term_substitution(quant_solver_t *solver, term_t *var, term_t *value, uint32_t n, term_t t) {
  term_subst_t subst;
  term_t g;
  int_hmap_pair_t *p;
  uint32_t i;
  term_t x;

  subst.mngr = solver->prob->manager;
  subst.terms = solver->prob->terms;
  init_int_hmap(&subst.map, 0);
  init_subst_cache(&subst.cache);
  init_istack(&subst.stack);
  subst.rctx = NULL;

  for (i=0; i<n; i++) {
    x = var[i];
    p = int_hmap_get(&subst.map, x);
    p->val = value[i];

    assert(is_pos_term(x));
    assert(good_term(subst.terms, x));
    assert(term_kind(subst.terms, x) == VARIABLE);
    assert(good_term(subst.terms, p->val));
  }

  g = apply_term_subst(&subst, t);
  delete_term_subst(&subst);

  return g;
}

/*
 * Find the term mapped to a given occurence
 */
term_t find_intern_mapping(intern_tbl_t *tbl, occ_t rhs) {
  term_table_t *terms;
  uint32_t i, n;
  term_t r;
  int32_t code;

  terms = tbl->terms;
  n = tbl->map.top;
  for (i=0; i<n; i++) {
    if (good_term_idx(terms, i) && intern_tbl_is_root_idx(tbl, i)) {
      r = pos_term(i);
      if (intern_tbl_root_is_mapped(tbl, r)) {
        code = intern_tbl_map_of_root(tbl, r);
        if (code_is_valid(code) &&
            code_is_eterm(code) &&
            code2occ(code) == rhs) {
          return r;
        }
      }
    }
  }

  return NULL_TERM;
}


/*
 * Instantiate constraint cnstr with match at index idx
 */
bool ematch_cnstr_instantiate(quant_solver_t *solver, quant_cnstr_t *cnstr, pattern_t *pat, uint32_t idx) {
  int_hset_t *instances;
  ematch_globals_t *em;
  context_t *ctx;
  intern_tbl_t *intern;
  instance_table_t *instbl;
  instance_t * inst;
  term_t t;
  uint32_t i, n, pol;
  term_t x;
  occ_t rhs;

  instances = &cnstr->instances;
  if (int_hset_member(instances, idx)) {
#if TRACE
    printf("\n  already done with match%d\n", idx);
#endif
//    assert(0);
    return false;
  }

  em = &solver->em;
  ctx = em->ctx;
  intern = &ctx->intern;
  instbl = &em->instbl;
  t = cnstr->t;

  assert(idx < instbl->ninstances);
  inst = &instbl->data[idx];

#if TRACE
  printf("\n  generating instance with match%d\n", idx);
#endif

#if 0
  printf("\nInstantiating %s\n", yices_term_to_string(t, 120, 1, 0));
#endif
//  assert(is_boolean_term(ctx->terms, t));
//  assert(intern_tbl_is_root(&ctx->intern, t));
//  assert(term_is_true(ctx, t));

  n = inst->nelems;
  assert(n == iv_len(pat->pvars));

  term_t *values = (term_t *) safe_malloc(n * sizeof(term_t));
  for(i=0; i<n; i++) {
    rhs = inst->odata[i];
    pol = polarity_of_occ(rhs);
    rhs = pos_occ(term_of_occ(rhs));

    x = find_intern_mapping(intern, rhs);
    if(pol) {
      x = opposite_term(x);
    }
    values[i] = x;
    assert(x != NULL_TERM);
#if TRACE
    printf("reverse map: ");
    print_occurrence(stdout, rhs);
    printf(" --> ");
    yices_pp_term(stdout, x, 120, 1, 0);
    printf("\n");
#endif
  }

  t = term_substitution(solver, pat->pvars, values, n, t);
#if TRACE
    printf("Instance: ");
    yices_pp_term(stdout, t, 120, 1, 0);
    printf("\n");
#endif

  int32_t code = setjmp(ctx->env);
  if (code == 0) {
    quant_assert_toplevel_formula(ctx, t);
    int_hset_add(instances, idx);
  } else {
    printf("Failure in ematch instantiation with code: %d\n", code);
    exit(1);
  }

  return true;
}

/*
 * Match and learn instances for a given cnstr at index idx
 */
static void ematch_process_cnstr(quant_solver_t *solver, uint32_t idx) {
  ematch_globals_t *em;
  ematch_exec_t *exec;
  pattern_table_t *ptbl;
  quant_table_t *qtbl;
  uint32_t npat, i, j, n;
  quant_cnstr_t *cnstr;
  int32_t *patterns;
  pattern_t *pat;
  ptr_hmap_t *matches;
  ptr_hmap_pair_t *p;
  ivector_t *v;

  em = &solver->em;
  exec =  &em->exec;
  qtbl = em->qtbl;
  ptbl = em->ptbl;

  cnstr = qtbl->data + idx;

#if TRACE
  uint32_t oldcount;
  oldcount = solver->num_instances;

  printf("-------------------\n");
  printf("Trying matching cnstr @%d: ", idx);
  yices_pp_term(stdout, cnstr->t, 120, 1, 0);
#endif

  patterns = cnstr->patterns;
  npat = iv_len(patterns);
  if (npat != 0) {
    for(j=0; j<npat; j++) {
      pat = ptbl->data + patterns[j];

#if TRACE
      printf("\n  Matching pattern @%d: ", patterns[j]);
      yices_pp_term(stdout, pat->p, 120, 1, 0);
#endif

      ematch_exec_pattern(exec, pat, &cnstr->instances);

      matches = &pat->matches;
      for (p = ptr_hmap_first_record(matches);
           p != NULL;
           p = ptr_hmap_next_record(matches, p)) {
        v = p->val;
        assert(v != NULL);
        n = v->size;
        for(i=0; i<n; i++) {
          if (ematch_cnstr_instantiate(solver, cnstr, pat, v->data[i])) {
            solver->num_instances++;
            if(solver->num_instances == solver->max_instances) {
#if TRACE
              printf("\nReached max limit of #%d instances\n", solver->max_instances);
#endif
              goto done;
            }
          }
        }
      }
    }
  }

done:
#if TRACE
  if((solver->num_instances - oldcount) != 0) {
    printf("\nFound #%d instances for cnstr @%d\n", (solver->num_instances - oldcount), idx);
  }
#endif
  return;
}

/*
 * Match and learn instances
 */
static void ematch_process_all_cnstr(quant_solver_t *solver) {
  quant_table_t *qtbl;
  uint32_t i, n;

  qtbl = &solver->qtbl;
  n = qtbl->nquant;

  for(i=0; i<n; i++) {
    ematch_process_cnstr(solver, i);
    if (solver->num_instances == solver->max_instances)
      return;
  }
}



/**********************
 *  STATISTIC RECORD  *
 *********************/

static void init_quant_solver_statistics(quant_solver_stats_t *stat) {
  stat->num_quantifiers = 0;
  stat->num_patterns = 0;
  stat->num_instances = 0;
}

static inline void reset_quant_solver_statistics(quant_solver_stats_t *stat) {
  init_quant_solver_statistics(stat);
}


/*****************
 *  FULL SOLVER  *
 ****************/

/*
 * Initialization
 * - core = attached smt_core
 * - gates = gate manager for the core
 * - egraph = attached egraph
 * - ttbl = attached type table
 */
void init_quant_solver(quant_solver_t *solver, smt_core_t *core,
                     gate_manager_t *gates, egraph_t *egraph, type_table_t *ttbl) {

  solver->core = core;
  solver->gate_manager = gates;
  solver->egraph = egraph;
  solver->types = ttbl;

  solver->base_level = 0;
  solver->decision_level = 0;

  init_quant_solver_statistics(&solver->stats);

  solver->max_instances = DEFAULT_MAX_INSTANCES;

  solver->prob = NULL;
  init_pattern_table(&solver->ptbl);
  init_quant_table(&solver->qtbl);
  init_ematch(&solver->em);

  init_ivector(&solver->aux_vector, 10);
  init_int_hmap(&solver->aux_map, 0);
  init_ivector(&solver->lemma_vector, 10);

}


/*
 * Delete the solver
 */
void delete_quant_solver(quant_solver_t *solver) {
  delete_pattern_table(&solver->ptbl);
  delete_quant_table(&solver->qtbl);
  delete_ematch(&solver->em);

  delete_ivector(&solver->aux_vector);
  delete_int_hmap(&solver->aux_map);
  delete_ivector(&solver->lemma_vector);
}


/*
 * Reset
 */
void quant_solver_reset(quant_solver_t *solver) {
  solver->base_level = 0;
  solver->decision_level = 0;
  reset_quant_solver_statistics(&solver->stats);

  solver->prob = NULL;
  reset_pattern_table(&solver->ptbl);
  reset_quant_table(&solver->qtbl);
  reset_ematch(&solver->em);

  ivector_reset(&solver->aux_vector);
  int_hmap_reset(&solver->aux_map);
  ivector_reset(&solver->lemma_vector);
}


/*
 * Increase the decision level
 */
void quant_solver_increase_decision_level(quant_solver_t *solver) {
  uint32_t k;

  k = solver->decision_level + 1;
  solver->decision_level = k;
}


/*
 * Backtrack to back_level:
 */
void quant_solver_backtrack(quant_solver_t *solver, uint32_t back_level) {
  assert(solver->base_level <= back_level && back_level < solver->decision_level);
  solver->decision_level = back_level;
}


/*
 * Push:
 */
void quant_solver_push(quant_solver_t *solver) {
  assert(solver->base_level == solver->decision_level);

  solver->base_level ++;
  quant_solver_increase_decision_level(solver);
  assert(solver->base_level == solver->decision_level);
}


/*
 * Pop:
 */
void quant_solver_pop(quant_solver_t *solver) {
  assert(solver->base_level > 0 && solver->base_level == solver->decision_level);

  solver->base_level --;

  quant_solver_backtrack(solver, solver->base_level);
}


/*
 * Prepare for internalization: do nothing
 */
void quant_solver_start_internalization(quant_solver_t *solver) {
}

/*
 * Start search
 */
void quant_solver_start_search(quant_solver_t *solver) {
#if TRACE
  printf("\n=== START SEARCH ===\n");
  printf("\n\n");
#endif
}


/*
 * Propagate: do nothing
 * - all the work is done in final_check
 */
bool quant_solver_propagate(quant_solver_t *solver) {
  return true;
}



/*
 * FINAL CHECK: deal only with quantifier instantiation.
 *
 */
fcheck_code_t quant_solver_final_check(quant_solver_t *solver) {
#if TRACE
  printf("\n**** QUANTSOLVER: FINAL CHECK ***\n\n");
#endif

//#if TRACE
//  print_egraph_terms(stdout, solver->egraph);
////  print_egraph_terms_details(stdout, solver->egraph);
//  printf("\n\n");
//  print_egraph_root_classes(stdout, solver->egraph);
////  print_egraph_root_classes_details(stdout, solver->egraph);
//#endif
//
//  printf("\n(BEGIN) Intern. mappings:\n");
//  print_context_intern_mapping(stdout, solver->em.ctx);

//  ematch_execute_all_patterns(&solver->em);

  solver->num_instances = 0;
  ematch_process_all_cnstr(solver);

//#if TRACE
//  print_egraph_terms(stdout, solver->egraph);
////  print_egraph_terms_details(stdout, solver->egraph);
//  printf("\n\n");
//  print_egraph_root_classes(stdout, solver->egraph);
////  print_egraph_root_classes_details(stdout, solver->egraph);
//#endif
//
//  printf("\n(END) Intern. mappings:\n");
//  print_context_intern_mapping(stdout, solver->em.ctx);

#if EM_VERBOSE
  if (solver->num_instances != 0) {
    printf("EMATCH: learnt %d instances\n", solver->num_instances);
  }
#endif

#if TRACE
  printf("\n**** QUANTSOLVER: FINAL CHECK DONE ***\n\n");
#endif

  return (solver->num_instances==0) ? FCHECK_SAT : FCHECK_CONTINUE;
}


/*
 * Clear: nothing to do
 */
void quant_solver_clear(quant_solver_t *solver) {
}



/***************************
 *  INTERFACE DESCRIPTORS  *
 **************************/

/*
 * Control and generic interface for the egraph
 */
static th_ctrl_interface_t fsolver_control = {
  (start_intern_fun_t) quant_solver_start_internalization,
  (start_fun_t) quant_solver_start_search,
  (propagate_fun_t) quant_solver_propagate,
  (final_check_fun_t) quant_solver_final_check,
  (increase_level_fun_t) quant_solver_increase_decision_level,
  (backtrack_fun_t) quant_solver_backtrack,
  (push_fun_t) quant_solver_push,
  (pop_fun_t) quant_solver_pop,
  (reset_fun_t) quant_solver_reset,
  (clear_fun_t) quant_solver_clear,
};

static th_egraph_interface_t fsolver_egraph = {
  NULL, // (assert_eq_fun_t) quant_solver_assert_var_eq,
  NULL, // (assert_diseq_fun_t) quant_solver_assert_var_diseq,
  NULL, // (assert_distinct_fun_t) quant_solver_assert_var_distinct,
  NULL, // (check_diseq_fun_t) quant_solver_check_disequality,
  NULL, // (is_constant_fun_t) quant_solver_var_is_constant,
  NULL, // no need for expand_th_explanation
  NULL, // (reconcile_model_fun_t) quant_solver_reconcile_model,
  NULL, // (prepare_model_fun_t) quant_solver_prepare_model,
  NULL, // (equal_in_model_fun_t) quant_solver_var_equal_in_model,
  NULL, // (gen_inter_lemma_fun_t) quant_solver_gen_interface_lemma, // gen_interface_lemma
  NULL, // (release_model_fun_t) quant_solver_release_model,
  NULL, // build_model_partition,
  NULL, // release_model_partition,
  NULL, // (attach_to_var_fun_t) quant_solver_attach_eterm,
  NULL, // (get_eterm_fun_t) quant_solver_get_eterm_of_var,
  NULL, // (select_eq_polarity_fun_t) quant_solver_select_eq_polarity,
};


/*
 * Quant-theory interface for the egraph
 */
static quant_egraph_interface_t fsolver_quant_egraph = {
  NULL, // (make_quant_var_quant_t) quant_solver_create_var,
};



/*
 * Access to the interface descriptors
 */
th_ctrl_interface_t *quant_solver_ctrl_interface(quant_solver_t *solver) {
  return &fsolver_control;
}

th_egraph_interface_t *quant_solver_egraph_interface(quant_solver_t *solver) {
  return &fsolver_egraph;
}

quant_egraph_interface_t *quant_solver_quant_egraph_interface(quant_solver_t *solver) {
  return &fsolver_quant_egraph;
}

