main 
	if talk
	then


		if quest * 1 * 0
		then
			speak " Allez tuer les gros m�chants pour avoir la tiare ! "
		end
		if quest * 1 * 1
		then
			speak " Allez tuer les gros m�chants pour avoir la tiare ! Il en reste encore 2 ! "
		end
		if quest * 1 * 2
		then
			speak " Allez tuer les gros m�chants pour avoir la tiare ! Il en reste encore 1 ! "
		end

		if quest * 1 * 3
		then
			speak " Bien jou�, vous les avez �limin�s, voici la tiare !"
			setQuestState * 1 * 4 " Qu�te termin�e."
			giftItem * 0
		end
		
		if quest * 1 * -1
		then
			if variable * 0 * 0
			then
				playSound * 1
				variable * 0 * 1
			end

			speak " Si vous allez tuer les trois gros m�chants pr�tres d�moniaques du donjon, je vous offrirai une belle tiare ! "
			newQuest * 1 
			setQuestName * 1 " Les trois m�chants"
			setQuestState * 1 * 0 " Allez tuer les trois m�chants pr�tres d�moniaques dans le donjon. Mais attention, ils sont tr�s puissants ! "
		end
	else
		variable * 0 * 0
	end
end 
