<?php

if ( isset($_GET['direction']) )
{
	/* Open the file */
	$myfile = fopen("status.txt","w");

	if ( $_GET['direction'] == 'right' )
	{
		fwrite($myfile,'r');
	}
	
	else if ( $_GET['direction'] == 'left' )
	{
		fwrite($myfile,'l');
	}
	else if ( $_GET['direction'] == 'forward' )
	{
		fwrite($myfile,'f');
	}
	else if ( $_GET['direction'] == 'back' )
	{
		fwrite($myfile,'b');
	}
	else{
		fwrite($myfile,'s');
	}
	/* Close file */
	fclose($myfile);
	header("Location: hostindex.html");
}



?>