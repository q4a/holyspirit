
main 
	if distance * 2
	then
		if variable * 0 * 0
		then
			
			variable * 0 * 1
			speak " Bien ! Maintenant que vous avez chang� de niveau, retournez dans l'inventaire. Vous pourrez d�penser vos points de caract�ristiques fraichement acquis en cliquant sur le petit '+' � c�t� de la caract�ristique voulue.  " 
		end
		
		if variable * 0 * 1
		then
			if no_speak
			then
				variable * 0 * 2
				speak "	-Votre force augmente vos d�gats au corps � corps et vous permet de porter de l'�quipement plus lourd.
						-Votre dext�rit� augmente vos d�g�ts � distance, vous permet de mieux toucher/esquiver et de porter plus d'�quipements.
						-Votre vitalit� augmente votre vie. 
						-Votre pi�t� est votre capacit� � invoquer votre Dieu, elle vous permet de lancer plus de miracles. (Nous allons y revenir.) 
						-Votre charisme influe sur le prix des objets et vous permet de porter certains �quipements. "
			end
		end
	end
	
end 
