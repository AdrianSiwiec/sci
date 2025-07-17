# Synthetic tests
./generateSyntheticData.sh
make benchmark
./obj/test/benchmark.e <benchmark_data/synthetic.in >benchmark_out/synthetic.out
./obj/test/benchmark.e <benchmark_data/synthetic_closed.in >benchmark_out/synthetic_closed.out
./obj/test/benchmark.e <benchmark_data/synthetic_tautologies.in >benchmark_out/synthetic_tautologies.out

# Hilbert tautologies
make benchmark
./obj/test/benchmark.e <benchmark_data/hilbert_out_backup_found.formulas >benchmark_out/out.csv