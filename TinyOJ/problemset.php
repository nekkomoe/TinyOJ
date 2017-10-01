<?php
	header("Content-type: text/html; charset=utf-8");
	require "tool.php";

	// 题目列表

	$st = $_GET["start"] ? $_GET["start"] : 1000;
	$en = $_GET["end"] ? $_GET["end"] : $st;
	func_show_problem($st, $en);
?>