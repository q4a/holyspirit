main 

	if variable * 9 * 0
	then

		if no_speak
		then
			variable * 2 * 0 //AJOUTER CHOIX
			variable * 3 * 0 //MOMENT DE LA DISCUTION
			variable * 5 * 0
		end
		
		if variable * 0 * 0
		then
			if talk
			then
				if variable * 3 * 0
				then
					if speak_choice * 1
					then
						variable * 3 * 1
						variable * 2 * 0
						stop_speak
					end
				end
				
				if variable * 3 * 1
				then
					if speak_choice * 1
					then
						variable * 3 * 2
						variable * 2 * 0
						stop_speak
					end
					if speak_choice * 2
					then
						stop_speak
						stop_talk
					end
					if speak_choice * 3
					then
						variable * 3 * 3
						variable * 2 * 0
						stop_speak
						stop_talk
					end
				end
				
				if variable * 3 * 2
				then
					if speak_choice * 2
					then
						stop_speak
						stop_talk
					end
					if speak_choice * 1
					then
						variable * 3 * 4
						variable * 2 * 0
						stop_speak
						stop_talk
					end
				end
				
				if variable * 2 * 0
				   variable * 3 * 1
				then
					speak * 201
					speak_choice * 205 * 1
					speak_choice * 204 * 2
					speak_choice * 203 * 3
					variable * 2 * 1
				end
				
				if variable * 2 * 0
				   variable * 3 * 2
				then
					speak * 206
					speak_choice * 207 * 1
					speak_choice * 208 * 2
					variable * 2 * 1
				end
				
				if variable * 2 * 0
				   variable * 3 * 3
				then
					newQuest * 8
					setQuestName * 8 * 38
					setQuestState * 8 * 2 * 41
					setQuestActif * 8 * 0
					stopMiracle * 0
					
					variable * 0 * 2
					tell * 210 * 5000
				end
				
				if variable * 2 * 0
				   variable * 3 * 4
				then
					newQuest * 8
					setQuestName * 8 * 38
					setQuestState * 8 * 0 * 39
					stopMiracle * 0
					
					variable * 0 * 1
					tell * 209 * 5000
				end
				
				if variable * 2 * 0
				   variable * 3 * 0
				then
					speak * 200
					speak_choice * 202 * 1
					
					variable * 2 * 1
				end
			end
		end
		
		if variable * 0 * 1
		then
			stop_speak
			
			if see
			then
				fight
			else
				randomDisplace
			end
		end
		
		if variable * 0 * 2
		then
			setFriendly * 0
			
			stop_speak
		
			if alive
			then
				if see
				then
					fight
				else
					randomDisplace
				end
			else
				if variable * 7 * 0
				then
					variable * 7 * 1
				end
			end
		end
		
		if miracle * 0
		then
		else
			variable * 8 * 0
		end
		
		if quest * 8 * -1
			variable * 8 * 0
		then
			variable * 8 * 1
			useMiracle * 0
		end
	else
		setActif * 0
		
		if quest * 8 * 1
		then else
			setQuestState * 8 * 1 * 40
			setQuestActif * 8 * 0
		end
		
		if variable * 7 * 0
		then
			variable * 7 * 1
			tell * 211 * 5000
		end
	end
	
end 
