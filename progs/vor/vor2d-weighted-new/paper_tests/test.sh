for file in test_*
do
    ../vor2d "$file" --save=true
    ../vor2d "$file" --save=true --geps=0.001 --grid=false
done
