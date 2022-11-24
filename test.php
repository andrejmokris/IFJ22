<?php
declare(strict_types=1);
// Program 1: Vypocet faktorialu (iterativne)
// Hlavni telo programu
function add(int $x, int $y) : int {
    $a = $x + $y;
    return $a;
}

$x = readi();
$y = readi();
$res = add($x,$y);
$res = 2 * $res;
write($res);