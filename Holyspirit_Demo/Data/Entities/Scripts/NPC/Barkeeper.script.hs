main 
	if talk
	then
		if quest * 4 * -1
		then
			if variable * 0 * 0
			then
				speak * 43
				speak_choice * 44 * 1
				speak_choice * 45 * 2
				variable * 0 * 1
			end
		
			if speak_choice * 1
			then	
				stop_speak
				newQuest * 4 
				setQuestName * 4  * 47
				setQuestState * 4 * 0 * 48
			end
		
			if speak_choice * 2
			then
				stop_speak
			end
		else
			if quest * 4 * 1
			then
				speak * 52
				setQuestState * 4 * 2 * 51
				addCash * 1000
			end
		end
	else
		variable * 0 * 0
	end
end 
