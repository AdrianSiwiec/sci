a=1
while :
do
  echo "test $a"
  a=$((a+1))
  echo $RANDOM $RANDOM 1000 >seed
  ../obj/test/formula_generator.e <seed >in
  time ../obj/test/simple_solver.e <in >out_mine
  cat in | sed 's/^/S\n/g' | sed 's/→/->/g' | sed 's/¬/ not /g' | sed 's/≡/=/g' | tail -n +2 >in_hs
  time ../../tableaux-2021/SCIMain <in_hs >out_hs
  cat in | tail -n +2 >in2
  paste -d '\n' in2 out_mine >nice_out_mine
  paste -d '\n' in2 out_hs >nice_out_hs
  if ! diff -q nice_out_mine nice_out_hs &>/dev/null; then
    echo "ERROR ERROR ERROR"
    echo $(diff nice_out_mine nice_out_hs)
    exit 1
  fi
done