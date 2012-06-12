main 
	if quest * 5 * -1
	then
		if see
		then
		else
			tell * 137 * 5000
		end
	end

	if talk
	then
		if quest * 5 * -1
		then
			if variable * 0 * 0
			then
				speak * 138
				speak_choice * 139 * 1
				speak_choice * 140 * 2
				variable * 0 * 1
			end
		
			if speak_choice * 1
			then	
				stop_speak
				newQuest * 5 
				setQuestName * 5  * 25
				setQuestState * 5 * 0 * 26
			end
		
			if speak_choice * 2
			then
				stop_speak
			end
		else
			if quest * 5 * 1
			then
				speak * 142
				setQuestState * 5 * 2 * 28
				setQuestActif * 5 * 0
				setActif * 0
			end
		end
	else
		variable * 0 * 0
	end
end 
