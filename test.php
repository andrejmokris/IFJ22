<?php
declare(strict_types=1);

function square(int $a) : int {
    $x = $a * $a;
    return $x;
}

$cnt = readi();
$end = readi();
$x = 0;
while($cnt !== $end + 1) {
    $x = square($cnt);
    $cnt = $cnt + 1;
}
