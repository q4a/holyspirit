main 
	if talk
	then


		if quest * 1 * 0
		then
			speak " Allez tuer les gros méchants pour avoir la tiare ! "
		end
		if quest * 1 * 1
		then
			speak " Allez tuer les gros méchants pour avoir la tiare ! Il en reste encore 2 ! "
		end
		if quest * 1 * 2
		then
			speak " Allez tuer les gros méchants pour avoir la tiare ! Il en reste encore 1 ! "
		end

		if quest * 1 * 3
		then
			speak " Bien joué, vous les avez éliminés, voici la tiare !"
			setQuestState * 1 * 4 " Quête terminée."
			giftItem * 0
		end
		
		if quest * 1 * -1
		then
			if variable * 0 * 0
			then
				playSound * 1
				variable * 0 * 1
			end

			speak " Si vous allez tuer les trois gros méchants prêtres démoniaques du donjon, je vous offrirai une belle tiare ! "
			newQuest * 1 
			setQuestName * 1 " Les trois méchants"
			setQuestState * 1 * 0 " Allez tuer les trois méchants prêtres démoniaques dans le donjon. Mais attention, ils sont très puissants ! "
		end
	else
		variable * 0 * 0
	end
end 
