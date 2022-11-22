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
$cnt = 0;
while($cnt !== $x + 1) {
    $res = factorial($cnt);
    write($cnt,"\\032","VYSLEDOK:","\\032",$res, "\\010");
    $cnt = $cnt + 1;
}