make formulaGenerator


# # Random
# rm benchmark_data/synthetic.in
# touch benchmark_data/synthetic.in
# count=100
# for size in $(seq 3 50);
# do
#   for vars in $(seq 1 4);
#   do
#     obj/test/formula_generator.e $count $size $vars >>benchmark_data/synthetic.in
#   done
#   echo $size done for random!
# done


# Random closed only
# rm benchmark_data/synthetic_closed.in
touch benchmark_data/synthetic_closed.in
count=100
for vars in $(seq 1 4);
do
  # 6 is minimal size for a non-trivial closed formula (I guess)
  for size in $(seq 6 50);
  do
    if [ $size -le 7 ] && [ $vars -ge 3 ] ; then
      echo Skipping $vars $size 
      continue
    fi
    if [ $size -le 9 ] && [ $vars -ge 4 ] ; then
      echo Skipping $vars $size 
      continue
    fi
    obj/test/formula_generator.e $count $size $vars 1 >>benchmark_data/synthetic_closed.in
    echo $vars $size taut Done!
  done
done