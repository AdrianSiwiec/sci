# Synthetic tests
./generateSyntheticData.sh
make benchmark
./obj/test/benchmark.e <benchmark_data/synthetic.in >benchmark_out/synthetic.out

# Hilbert tautologies
make benchmark
./obj/test/benchmark.e <benchmark_data/hilbert_out_backup_found.formulas >benchmark_out/out.csv