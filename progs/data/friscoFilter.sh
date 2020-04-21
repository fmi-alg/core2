# !/bin/sh

############################################################
#
# Filter FRISCO format polynomials according to type
#
# They can be
#  dense or sparse  [d|s]
#  real or complex  [r|c]
#  integer or float or rational coefficients [i|f|q]
#
# Usage:
#	> friscoFilter.sh [r|c] [i|f|q]		-- takes 2 arguments
#	> friscoFilter.sh 			-- defaults to "r" and "i"
#
#   and it will list all the polynomials in the frisco database
#   with these characteristics
#
# Irina Voiculescu
# 13 June 2012, NYU
#
############################################################


# check first command line argument (complex or real)
if [[ $1 = r ]]; then
    echo "Printing names of real polynomials" $1
    decidecr=r
elif  [[ $1 = c ]]; then
    echo "Printing names of complex polynomials" $1
    decidecr=c
else
    echo "Usage " $0 "[c|r]  defaulting to r" "[i|f|q] defaulting to i"
    decidecr=r
fi

# check second command line argument (integer or rational)
if [[ $2 = i ]]; then
    echo "with integer coefficients" $2
    decideifq=i
elif  [[ $2 = f ]]; then
    echo "with floating point coefficients" $2
    decideifq=f
elif  [[ $2 = q ]]; then
    echo "with rational coefficients" $2
    decideifq=q
else
    echo "Usage " $0 "[c|r] defaulting to r" "[i|f|q] defaulting to i"
    decideifq=i
fi


# process every FRISCO polynomial in the frisco subdirectory
cd frisco
for x in *pol  ;
   { 
    # echo "Processing polynomial" $x "..."; 
	
    # Select first few lines of the polynomial file
    # ignore those that are comments
    # look for "c" in complex and for "r" in real polynomials
    # count (-c) whether "c" features in the current polynomial
    # head -5 $x | grep -v "\!" | grep -c "c"

    if [[ `head -5 $x | grep -v "\!" | grep -c $decidecr` -ne 0 ]]; then
	if [[ `head -5 $x | grep -v "\!" | grep -c $decideifq` -ne 0 ]]; then
            echo $x
	fi
    fi
   } 

cd ..

