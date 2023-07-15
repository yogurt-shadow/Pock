CC := g++ -std=c++17
P := polynomial
U := util

main: main.o  \
$P/algebraic_numbers.o $P/polynomial.o $P/polynomial_cache.o $P/rpolynomial.o $P/sexpr2upolynomial.o $P/upolynomial.o $P/upolynomial_factorization.o \
util/stack.o util/inf_rational.o util/approx_set.o util/env_params.o util/gparams.o util/util.o util/mpf.o util/luby.o util/common_msgs.o util/z3_exception.o util/page.o util/mpff.o util/statistics.o util/s_integer.o util/smt2_util.o util/region.o util/bit_vector.o util/min_cut.o util/rational.o util/mpn.o util/fixed_bit_vector.o util/hash.o util/warning.o util/approx_nat.o util/mpfx.o util/small_object_allocator.o util/sexpr.o util/inf_int_rational.o util/mpq.o util/bit_util.o util/mpbq.o util/permutation.o util/rlimit.o util/state_graph.o util/symbol.o util/lbool.o util/mpq_inf.o util/prime_generator.o util/cmd_context_types.o util/memory_manager.o util/scoped_ctrl_c.o util/tbv.o util/params.o util/timeit.o util/mpz.o util/hwf.o util/timeout.o util/zstring.o util/debug.o util/inf_s_integer.o util/scoped_timer.o util/trace.o util/lp_main.o
	$(CC) -O3 -o $@ $^ -lgmp -lm -lpthread

%.o: %.cpp
	$(CC) -c -O3 -o $@ $<

solve: main $P/polynomial.o 
	@echo ""
	@echo "###################################"
	@echo "# TEST: solve               #"
	@echo "###################################" 
	./main solve
	@echo ""
	@echo "###################################"
	@echo "# solve done!                   #"
	@echo "###################################"

clean:
	find . -name "*.o" | xargs rm -rf
	rm -f main