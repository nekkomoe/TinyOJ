<?php

	header("Content-type: text/html; charset=utf-8"); 

	require "tool.php";

	$problem_id = $_GET["problem_id"];

	if(!$problem_id) exit(0);

	$problem_content = file_get_contents($path_problem . strval($problem_id) . "/text.md");

	func_show_problem($problem_id, $problem_id);

?>

<!DOCTYPE html>
<html>
	<head>
	    <title>Problem <?php echo $problem_id;?></title>
	    <script type="text/javascript" src="js/showdown.min.js"></script>
	</head>
	<style>
	    #result {
	        background: #F6F6F6;
	    }
	</style>
	<body>
		<div>
		    <textarea id="oriContent" style="display:none;"><?php echo "$problem_content";?></textarea>
		    <div id="result"></div>
		</div>
		<script type="text/javascript">
			window.onload = function() {
				var text = document.getElementById("oriContent").value;
				var converter = new showdown.Converter();
				var html = converter.makeHtml(text);
				document.getElementById("result").innerHTML = html;
				[].forEach.call(document.querySelectorAll('*'),function(a){ a.style.outline="1px solid #"+(~~(Math.random()*(1<<24))).toString(16) });
			};
		</script>
	</body>
</html>