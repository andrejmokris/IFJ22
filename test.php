<?php
declare(strict_types=1);

function add(int $x, int $y) : int {
    $res  = $x + $y;
    return $res;
}

$x = add(2+(3*4),4);
write($x);
