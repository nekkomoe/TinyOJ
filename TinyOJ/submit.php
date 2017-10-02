<?php
	header("Content-type: text/html; charset=utf-8");
	require "tool.php";

	// 提交

	if($_POST["submit"] || $_POST["check"]) {
		func_judge(intval($_POST["problem_id"]), $_POST["code"]);
		exit(0);
	}
?>

<form action="" method="post">
<input type="hidden" name="check" value="1"/>
<input type="input" name="problem_id" placeholder="problem_id"></input><br/>
<textarea rows="25" cols="150" name="code" placeholder="// your code"></textarea><br/>
<input type="submit"></input>
</form>