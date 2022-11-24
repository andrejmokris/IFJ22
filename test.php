<?php
declare(strict_types=1);

$x = "xy" . "zw";
$y = substring($x, 1, 6);
write($y);