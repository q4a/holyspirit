main 
	if talk
	then


		if quest * 1 * 0
		then
			speak * 11
		end
		if quest * 1 * 1
		then
			speak * 12
		end
		if quest * 1 * 2
		then
			speak * 13
		end

		if quest * 1 * 3
		then
			speak * 15
			setQuestState * 1 * 4  * 17
			giftItem * 0
		end
		
		if quest * 1 * -1
		then
			if variable * 0 * 0
			then
				playSound * 1
				variable * 0 * 1
			end

			speak * 16
			newQuest * 1 
			setQuestName * 1  * 18
			setQuestState * 1 * 0  * 11
		end
	else
		variable * 0 * 0
	end
end 
