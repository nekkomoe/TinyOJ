<?php
	header("Content-type: text/html; charset=utf-8");
	require "tool.php";

	// 提交

	if($_POST["submit"]) {
		if(func_judge(intval($_POST["problem_id"]), $_POST["code"]) == true) {
			echo "ACCEPT";
		} else {
			echo "UNACCEPT";
		}
		exit(0);
	}
?>

<form action="" method="post">
<input type="input" name="problem_id" placeholder="problem_id"></input><br/>
<textarea rows="25" cols="150" name="code" placeholder="// your code"></textarea><br/>
<input type="submit"></input>
</form>