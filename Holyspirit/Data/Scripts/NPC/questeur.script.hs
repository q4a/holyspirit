main 
	if talk
	then
		if quest * 1 * 0
		then
			speak " Allez tuer les gros m�chants pour avoir la tiare !"
		end
		if quest * 1 * 1
		then
			speak " Allez tuer les gros m�chants pour avoir la tiare ! Il en reste encore 2 !"
		end
		if quest * 1 * 2
		then
			speak " Allez tuer les gros m�chants pour avoir la tiare ! Il en reste encore 1 !"
		end

		if quest * 1 * 3
		then
			speak " Vous les avez euts, voici la tiare !"
			setQuestState * 1 * 4 " Qu�te termin�e."
			giftItem * 0
		end
		
		if quest * 1 * -1
		then
			speak " Si vous allez tuer les trois gros m�chants du donjon, je vous offrirez une belle tiare ! "
			newQuest * 1 
			setQuestName * 1 " Les trois m�chants"
			setQuestState * 1 * 0 " Allez tuez les trois m�chant dans le donjon"
		end
	end
end 
