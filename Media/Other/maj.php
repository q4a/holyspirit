
<?php
mysql_connect("92.243.1.174", "greg", "1D49226C3D"); // Connexion à MySQL
mysql_select_db("CDN"); // Sélection de la base mateo21
?>


<? 
 $liste_rep = scandir("https://lechemindeladam.svn.sourceforge.net/svnroot/lechemindeladam/Holyspirit"); 
 $i = 2; 
 $num = count($liste_rep); 
	while($i < $num)
	{ 
		if(file_exists("https://lechemindeladam.svn.sourceforge.net/svnroot/lechemindeladam/Holyspirit/".$liste_rep[$i]))
		{
			$dejaExistant=false;
			while($donnees = mysql_fetch_array(mysql_query("SELECT * FROM liste_fichiers WHERE nom='".$liste_rep."'")))
				$dejaExistant=true;
			if($dejaExistant)
				mysql_query("UPDATE FROM liste_fichiers SET version='".filemtime("https://lechemindeladam.svn.sourceforge.net/svnroot/lechemindeladam/Holyspirit/".$liste_rep)."' WHERE nom='".$liste_rep."' ");
			else
				mysql_query("INSERT INTO liste_fichiers VALUE('".$liste_rep."','./','".filemtime("https://lechemindeladam.svn.sourceforge.net/svnroot/lechemindeladam/Holyspirit/".$liste_rep)."') ");
		}
		$i++; 
	} 

?> 
