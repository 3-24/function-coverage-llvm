set -e
set -o xtrace

rm -f main main.bc main_cov.bc main_cov

gclang -O0 -g  main.cc -o main 
get-bc main

opt -enable-new-pm=0 -load ../lib/libprintfunc.so --PrintFunc -o main_cov.bc < main.bc
llvm-dis-13 main_cov.bc
clang++ main_cov.bc -o main_cov -L../lib -lpf-rt
./main_cov

echo ""
echo "Coverage result:"
cat main.cc.cov
