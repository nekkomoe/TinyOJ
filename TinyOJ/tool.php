<?php

	header("Content-type: text/html; charset=utf-8"); 

	$file_problem_lists = "problem_lists.ini";

	$flag_show_problem_tag = true;

	$path_judge = "judge/";

	$file_judge_default_file_name = "demo";

	$file_judge_default_file_suffix = ".cpp";

	function func_add_problem($problem_id) {
		global $file_problem_lists;
		$tmp = explode(";", file_get_contents($file_problem_lists));
		if(!is_writable($file_problem_lists)) {
			echo "need to 'chmod 777 $file_problem_lists'";
			exit(0);
		}
		file_put_contents($file_problem_lists, strval($problem_id) . PHP_EOL, FILE_APPEND);
	}

	function func_show_problem($start_id, $end_id) {
		global $file_problem_lists, $flag_show_problem_tag;
		
		// 读取题目编号并确定查询下标
		$problem_arr = explode(PHP_EOL, file_get_contents($file_problem_lists));
		while(count($problem_arr) && !is_numeric(end($problem_arr))) {
			array_pop($problem_arr);
		}
		for($i = 0 ; $i < count($problem_arr) ; ++ $i) {
			$problem_arr[$i] = intval($problem_arr[$i]);
		}
		$problem_arr = array_unique($problem_arr);
		sort($problem_arr);
		$st = -1;
		$en = -1;
		for($i = 0 ; $i < count($problem_arr) ; ++ $i) {
			if($start_id <= $problem_arr[$i]) {
				$st = $i;
				break;
			}
		}
		for($i = count($problem_arr) - 1 ; $i >= 0 ; -- $i) {
			if($end_id >= $problem_arr[$i]) {
				$en = $i;
				break;
			}
		}
		if($st == -1 || $en == -1) {
			return;
		}

		// 输出查询
		for($i = $st ; $i <= $en ; ++ $i) {
			// 显示标签
			$problem_id = $problem_arr[$i];
			$json_problem_ini = json_decode(file_get_contents($problem_id . "/problem.ini"));

			echo $json_problem_ini -> id . "<br/>";
			echo $json_problem_ini -> title . "<br/>";
			echo "input_file_name : " . $json_problem_ini -> data_name . $json_problem_ini -> input_data_suffix . "<br/>";
			echo "output_file_name : " . $json_problem_ini -> data_name . $json_problem_ini -> output_data_suffix . "<br/>";
			echo "time : " . $json_problem_ini -> time . "ms" . "<br/>";
			echo "memory : " . $json_problem_ini -> memory . "kb" . "<br/>";
			if($flag_show_problem_tag) {
				echo "tag : " . $json_problem_ini -> tag;
			}
			echo "<br/><br/>";
		}
	}

	function func_judge($problem_id, $content) {
		global $path_judge, $file_judge_default_file_name, $file_judge_default_file_suffix;
		file_put_contents($path_judge . $file_judge_default_file_name . $file_judge_default_file_suffix, $content);

		if(!is_readable(strval($problem_id) . "/problem.ini")) {
			echo "can't judge";
			return;
		}

		func_show_problem($problem_id, $problem_id);

		$json_problem_ini = json_decode(file_get_contents($problem_id . "/problem.ini"));
		$cppName = "demo";
		$dataCount = $json_problem_ini -> data_count;
		$inpath = "../" . strval($problem_id) . "/input/";
		$inputName = $json_problem_ini -> data_name;
		$outputName = $json_problem_ini -> data_name;
		$outpath = "../" . strval($problem_id) . "/output/";
		$stdOutputName = $json_problem_ini -> data_name;
		$time = $json_problem_ini -> time / 1000;
		$ret = shell_exec("cd judge && ./judge $cppName $dataCount $inpath $inputName $outputName $outpath $stdOutputName $time");
		return substr_count($ret, "JD_ACCEPT") == 1;
	}

?>