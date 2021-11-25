#!/bin/zsh
autoload -U colors && colors

make

echo ""
echo ""
echo ""

for test in ./tests/in/*.in
do
    echo "$fg_bold[green]Running file $test"$reset_color
    echo "$fg_bold[yellow]========================================================="$reset_color
    ./main < $test && echo "$fg_bold[green]Test $test passed"$reset_color || echo "$fg_bold[red]Test $test failed"$reset_color
    echo "$fg_bold[yellow]========================================================="$reset_color
    echo ""
    notify-send -t 5000 -i "alacritty" "Test $test" "Completed"
done
