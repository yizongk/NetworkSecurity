<?php

$host="localhost";
$user="root";
$password="";


$conn = mysqli_connect($host,$user,$password);
$db = mysqli_select_db($conn,"users");

if(isset($_POST['username'])){
	$user=$_POST['username'];
	$password=$_POST['password'];

	$sql="SELECT * FROM login WHERE Username='$user' AND Password='$password'";

	$result=mysqli_query($conn, $sql);

	if(mysqli_num_rows($result)==1) {
		echo "Logged in";
		exit();
	} else {
		echo "Incorrect user/pass";
		exit();
	}
}
?>

<!DOCTYPE HTML>
<html>
	<head>
		<title> Login Page </title>
	</head>
		<body>
			<div class = "container">
				<form method="POST" action="#">
					<div class="form_input">
						<input type="text" name="username" placeholder="Enter your username"/>
					</div>
					<div class="form_input">
						<input type="password" name="password" placeholder="Enter your password"/>
					</div>
					<input type="submit" name="submit" value="LOGIN" class="btn-login"/>
				</form>
			</div>
		</body>
</html>