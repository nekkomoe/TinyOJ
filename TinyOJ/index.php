<?php

require "tool.php";

func_add_problem(1000);

func_show_problem(1000, 1000);

func_judge(1000, file_get_contents("aplusb.cpp"));

?>