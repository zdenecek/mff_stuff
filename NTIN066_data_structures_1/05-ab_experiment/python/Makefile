STUDENT_ID ?= 98

.PHONY: test
test: ab_experiment.py ab_tree.py
	@rm -rf out && mkdir out
	@for test in insert min random ; do \
		for mode in '2-3' '2-4' ; do \
			echo t-$$test-$$mode ; \
			./ab_experiment.py $$test $(STUDENT_ID) $$mode >out/t-$$test-$$mode ; \
		done ; \
	done

.PHONY: clean
clean::
	rm -rf out __pycache__
