
main 
	if distance * 2
	then
		if variable * 0 * 0
		then
			
			variable * 0 * 1
			speak " Bien ! Maintenant que vous avez changé de niveau, retournez dans l'inventaire. Vous pourrez dépenser vos points de caractéristiques fraichement acquis en cliquant sur le petit '+' à côté de la caractéristique voulue.  " 
		end
		
		if variable * 0 * 1
		then
			if no_speak
			then
				variable * 0 * 2
				speak "	-Votre force augmente vos dégats au corps à corps et vous permet de porter de l'équipement plus lourd.
						-Votre dextérité augmente vos dégâts à distance, vous permet de mieux toucher/esquiver et de porter plus d'équipements.
						-Votre vitalité augmente votre vie. 
						-Votre piété est votre capacité à invoquer votre Dieu, elle vous permet de lancer plus de miracles. (Nous allons y revenir.) 
						-Votre charisme influe sur le prix des objets et vous permet de porter certains équipements. "
			end
		end
	end
	
end 
