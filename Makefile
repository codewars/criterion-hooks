criterion:
	mkdir criterion
	curl -sSL https://github.com/Snaipe/Criterion/releases/download/v2.3.3/criterion-v2.3.3-linux-x86_64.tar.bz2 | tar xj -C criterion --strip-components=1

criterion-hooks.o: criterion
	clang -Icriterion/include/ -c criterion-hooks.c -Wall -Wextra

example-test: criterion-hooks.o
	clang -Icriterion/include/ -Lcriterion/lib -lcriterion example/solution.c example/tests.c criterion-hooks.o -o example-test -Wall -Wextra

run-example: example-test
	@LD_LIBRARY_PATH=criterion/lib:$$LD_LIBRARY_PATH ./example-test -q -j1 --full-stats
.PHONY: run-example

clean:
	rm -rf ./criterion ./criterion-hooks.o ./test
.PHONY: clean
