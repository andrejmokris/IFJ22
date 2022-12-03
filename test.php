<?php 
declare(strict_types=1);

function fun(int $x) : int {
    $x = readi();

    if($x)
    {
        return 1;
    }
    else
    {
        return "a";
    }
}

fun(2);