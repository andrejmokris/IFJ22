<?php
declare(strict_types=1);

function hello(int $x) : ?string {
    if ($x === 0) {
        return "kokot\n";
    }
}

$x = hello(0);
write($x);