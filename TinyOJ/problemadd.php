<?php
    
	header("Content-type: text/html; charset=utf-8");
	require "tool.php";

	// 新建题目

    if($_POST["submit"]){
        $tmpname=$_FILES["filename"]["tmp_name"];
        $filename=$_FILES["filename"]["name"];
        $file_tar = $path_problem . $filename;
        move_uploaded_file($tmpname, $file_tar);
        $problem_id = intval($_FILES["filename"]["name"]);
        exec("cd $path_problem && tar -xf $filename");
        func_add_problem($problem_id);
        func_show_problem($problem_id, $problem_id);
        exit(0);
    }
?>


<form action="" method="post" enctype="multipart/form-data">
    <input type="file" name="filename"/>
    <input type="submit" name="submit" value="上传文件(需要是tar文件)并解压"/>
</form>