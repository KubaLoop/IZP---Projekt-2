#!/bin/bash
# author:   ivasicek@fit.vutbr.cz
# date:     2023

#-------------------------------------------------------------------------------
# HOW TO USE
# - This script will compile your project and then will check its outputs
#   on a few examples.
# - Needs to run on a Unix system (tested on merlin.fit.vutbr.cz and on the
#   Ubuntu subsystem for Windows)
# - Place this script into an EMPTY directory and COPY your project ("maze.c")
#   into the same directory (to avoid accidentally deleting your project or
#   other files).
# - The script needs to be executable. Run this to add executable permissions:
#      $ chmod +x ./maze-test.sh 
# - Execute the script like this:
#      $ ./maze-test.sh
# - The output of the script will be OK / FAILED for each test. The results 
#   directory will contain these files for each example:
#     ./results/exN-your_stdout.txt - standard output of your project
#     ./results/exN-your_stderr.txt - standard error output of your project
#     ./results/exN-ref_stdout.txt  - what the stdout should look like
#     ./results/exN-diff_out.txt    - diffence of your stdout and the ref stdout
#
# !! WARNING !!
# - This script will delete these two directories (relative to its directory):
#     ./inputs
#     ./results
#-------------------------------------------------------------------------------


# move to the script directory (in case someone runs it from outside the dir)
ROOTDIR="$(dirname "$(realpath "$0")")"
cd "$ROOTDIR"

# in-script saved contents of files
BLUDISTE="6 7\n1 4 4 2 5 0 6\n1 4 4 0 4 0 2\n1 0 4 0 4 6 1\n1 2 7 1 0 4 2\n3 1 4 2 3 1 2\n4 2 5 0 4 2 5\n"
BLUDISTE_INVALID="6 7\n1 4 4 2 5 0 6\n1 4 4 0 4 0 2\n1 0 4 0 4 6 1\n1 2 7 1 0 4 2\n3 1 4 2 3 1 2\n"     # invalid because the last row is missing
REF_STDOUT_EX1="Valid\n"
REF_STDOUT_EX2="Invalid\n"
REF_STDOUT_EX3="6,1\n6,2\n5,2\n5,3\n5,4\n6,4\n6,5\n6,6\n5,6\n5,7\n4,7\n4,6\n4,5\n5,5\n4,5\n4,4\n3,4\n3,3\n3,2\n4,2\n4,1\n5,1\n4,1\n4,2\n3,2\n3,1\n2,1\n2,2\n2,3\n2,4\n1,4\n1,3\n1,2\n1,1\n"
REF_STDOUT_EX4="6,7\n"
REF_STDOUT_EX5="6,1\n6,2\n5,2\n5,3\n5,4\n6,4\n6,3\n6,4\n6,5\n6,6\n5,6\n5,7\n4,7\n4,6\n4,5\n4,4\n3,4\n3,5\n3,6\n3,5\n3,4\n3,3\n3,2\n3,1\n2,1\n2,2\n2,3\n2,4\n2,5\n2,6\n2,7\n3,7\n"
REF_STDOUT_EX6="6,7\n"
REF_STDOUT_EX7="3,7\n2,7\n2,6\n2,5\n2,4\n1,4\n1,3\n1,2\n1,1\n"


# function to run a test and check its output
#   $1 ... execution arguments
#   $2 ... number of the example (to know which ref output to check)
test_example() {
    execArgs="$1"
    exampleN="$2"

    echo "  Example $exampleN: ./maze $execArgs"
    ./maze $execArgs 1> ./results/ex$exampleN-your_stdout.txt 2> ./results/ex$exampleN-your_stderr.txt
    diffOut=$(diff ./results/ex$exampleN-ref_stdout.txt ./results/ex$exampleN-your_stdout.txt 2>&1)
    ret=$?
    if [ $ret -ne 0 ]; then
        echo -e "      FAILED (see ./results/ex$exampleN-diff_out.txt)\n"
        echo "$diffOut" > ./results/ex$exampleN-diff_out.txt 
    else
        echo -e "      OK\n"
    fi
}

# clean outputs of the last run -- but issue a warning to avoid unintentionally 
# deleting something we're not supposed to (e.g. if a student uses ./inputs
# for their own tests)
if [ -d "./inputs" ] || [ -d "./results" ]; then
    echo "Warning: Directories \"./inputs\" and \"./results\" will now be deleted!"
    echo "         Press enter to continue. Or press CTRL+C to stop the script."
    read # wait for confirmation
fi
rm -rf ./inputs ./results

# make a directory for the outputs/results and a directory for the intputs
mkdir ./inputs
mkdir ./results

echo "----- Required functionality -----------------------------------"

# test compilation
echo "  Compilation: gcc -std=c11 -Wall -Wextra -Werror maze.c -o maze"
gcc -std=c11 -Wall -Wextra -Werror maze.c -o maze > ./results/compilation_out.txt 2>&1
ret=$?
if [ $ret -ne 0 ]; then
    echo -e "      FAILED (see ./results/compilation_out.txt)\n"
    exit
else
    echo -e "      OK\n"
fi

# generate input/output files (saved in-script to keep it as a single file)
echo -ne "$BLUDISTE" > ./inputs/bludiste.txt
echo -ne "$BLUDISTE_INVALID" > ./inputs/bludiste_invalid.txt
echo -ne "$REF_STDOUT_EX1" > ./results/ex1-ref_stdout.txt
echo -ne "$REF_STDOUT_EX2" > ./results/ex2-ref_stdout.txt
echo -ne "$REF_STDOUT_EX3" > ./results/ex3-ref_stdout.txt
echo -ne "$REF_STDOUT_EX4" > ./results/ex4-ref_stdout.txt
echo -ne "$REF_STDOUT_EX5" > ./results/ex5-ref_stdout.txt
echo -ne "$REF_STDOUT_EX6" > ./results/ex6-ref_stdout.txt
echo -ne "$REF_STDOUT_EX7" > ./results/ex7-ref_stdout.txt

# test examples from the assignment
test_example "--test ./inputs/bludiste.txt"          1
test_example "--test ./inputs/bludiste_invalid.txt"  2
test_example "--lpath 6 1 ./inputs/bludiste.txt"     3
test_example "--lpath 6 7 ./inputs/bludiste.txt"     4
test_example "--rpath 6 1 ./inputs/bludiste.txt"     5
test_example "--rpath 6 7 ./inputs/bludiste.txt"     6

# test the bonus functionality
echo "----- Bonus/Premium functionality ------------------------------"
test_example "--shortest 3 7 ./inputs/bludiste.txt"  7
