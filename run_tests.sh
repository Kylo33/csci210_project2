for test in 01 02 03 04 05 06 07 08 09 10; do
    diff -s --brief test_cases/test$test.out <(./main < test_cases/test$test.in 2>/dev/null)
done
