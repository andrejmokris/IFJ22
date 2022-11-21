<?php
declare(strict_types=1);

function factorial(int $n) : int {
    if ($n < 2) {
        $result = 1;
    } else {
        $decremented_n = $n - 1;
        $temp_result = factorial($decremented_n);
        $result = $n * $temp_result;
    }
    return $result;
}
write("FACTORIAL.NUMBER:");
$x = readi();
$res = factorial($x);
write("VYSLEDOK:", $res);