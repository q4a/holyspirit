
main 
	if distance * 2
	then
		if variable * 0 * 0
		then
			variable * 0 * 1
			speak " Maintenant que vous avez ramassé des objets, ouvrez votre inventaire avec le bouton en forme de casque en bas à gauche, ou bien appuyez sur 'I'.
					Pour prendre un objet en main, faites un clique gauche dessus, vous le déposerez en refaisant un clic gauche.
					Pour équiper un objet, placez-le dans la case correspondante à gauche.
					Vous pouvez aussi mettre des objets en raccourcis en les déposant dans la barre de raccourcis, en bas à gauche. Vous y accéderez en jeu en appuyant sur les touches 1 à 4. "
		end
		
		if variable * 0 * 1
		then
			if no_speak
			then
				variable * 0 * 2
				speak " Vous pourrez aussi remarquer que quand vous tuez des ennemis, vous gagnez des points d'âmes, représentés par le pentacle bleu qui grandit. Quand votre pentacle est rempli, vous gagnez un niveau.
						Tuez les ennemis dans la salle suivante pour changer de niveau. "
			end
		end
		
	end
end 
