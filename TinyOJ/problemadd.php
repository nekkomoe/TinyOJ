<?php
	header("Content-type: text/html; charset=utf-8");
	require "tool.php";

	// 新建题目

	if($_POST["check"] && is_numeric($_POST["problem_id"])) {
		func_add_problem(intval($_POST["problem_id"]));
	}
?>

<form action="problemadd.php" method="post">
<input type="hidden" name="check" value="true"></input>
<input type="input" name="problem_id" placeholder="problem_id"></input><br/>
<input type="submit"></input>
</form>