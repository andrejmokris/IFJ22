<?php
declare(strict_types=1);

function factorial(int $n) : int {
    if ($n === 1) {
        $result = 1;
    } else {
        $decremented_n = $n - 1;
        $temp_result = factorial($decremented_n);
        $result = $n * $temp_result;
    }
    return $result;
    }

$cnt = readi();
$res = factorial($cnt);

