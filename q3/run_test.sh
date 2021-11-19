#!/bin/zsh
autoload -U colors && colors

make

echo ""
echo ""
echo ""

for test in ./tests/in/*.in
do
    echo "$fg_bold[green]Running file $test"$reset_color
    ./client < $test
    echo ""
done
