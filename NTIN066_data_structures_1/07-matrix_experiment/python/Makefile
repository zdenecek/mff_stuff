TESTS=m1024-b16 m8192-b64 m65536-b256 m65536-b4096
TESTFILES=$(addprefix out/t-sim-,$(TESTS))

.PHONY: test
test: $(addsuffix -smart,$(TESTFILES)) $(addsuffix -naive,$(TESTFILES))

out/t-sim-%-naive:
	@mkdir -p out
	./matrix_experiment_sim.py $* naive >$@

out/t-sim-%-smart:
	@mkdir -p out
	./matrix_experiment_sim.py $* smart >$@

.PHONY: clean
clean::
	rm -rf out __pycache__
