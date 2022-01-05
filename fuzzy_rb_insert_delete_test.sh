#!/bin/bash

NUM_TESTS=15
TREE_SIZE_IN_TEST=2000

SRC_TEST_CODE='
#include <iostream>
#include <stdlib.h>
#include "rb_tree.hpp"

int main() {
	ft::RBTree<int> tree;

	srand (time(NULL));

	for (int j = 0; j < 3; j++) {
		for (int i = 0; i < TREE_SIZE_IN_TEST; i++) {
			tree.insert_node(rand() % TREE_SIZE_IN_TEST - (TREE_SIZE_IN_TEST / 2));
		}
		for (int i = 0; i < TREE_SIZE_IN_TEST; i++) {
			tree.delete_node(rand() % TREE_SIZE_IN_TEST - (TREE_SIZE_IN_TEST / 2));
		}
		// Output
		ft::RBTree<int>::const_iterator it;
		for (it = tree.begin(); it != tree.end(); ++it) {
			std::cout << *it << " ";
		}
		std::cout << std::endl;
		for (it = tree.end(); it != tree.begin(); --it) {
			if (it != tree.end()) {
				std::cout << *it << " ";
			}
		}
		std::cout << *it << " ";
		std::cout << std::endl;
	}
}
'

compile () {
	echo "$SRC_TEST_CODE" |
	clang++ -Wall -Wextra -Werror -o rb_fuzzy_test -D TREE_SIZE_IN_TEST=$TREE_SIZE_IN_TEST -x c++ -
}

test_fail () {
	rm rb_fuzzy_test
	echo Test Fail 🔥
	exit 1
}

no_program () {
	echo Missing necessery program 🔗: $1
	exit 1
}

test_dependencies () {
	for program in "$@"
	do
		if ! which -s $program
		then
			no_program $program
		fi
	done
}

test_dependencies clang++ valgrind

if ! compile
then
	echo Does not compile error
	test_fail
fi

for i in $(seq 1 1 $NUM_TESTS)
do
	if ! valgrind --leak-check=full --error-exitcode=1 ./rb_fuzzy_test > /dev/null 2>&1
	then
		test_fail
	else
		echo -n "⏳ "
	fi
done

rm rb_fuzzy_test

echo
echo Test Pass ✅
