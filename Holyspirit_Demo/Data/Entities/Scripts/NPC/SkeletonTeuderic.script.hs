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
						stop_talk
					end
					if speak_choice * 2
					then
						variable * 3 * 2
						variable * 2 * 0
						stop_speak
						stop_talk
					end
					if speak_choice * 3
					then
						stop_speak
						stop_talk
					end
				end
				
				
				if  variable * 5 * 0
				then
					if variable * 4 * 0 //PREMIERE FOIS
					then
						speak * 200
						variable * 4 * 1
					else
						if variable * 3 * 0
						then
							speak * 201
						end
					end
					variable * 5 * 1
				end
				
				if variable * 2 * 0
				   variable * 3 * 1
				then
					newQuest * 8
					setQuestName * 8 * 38
					setQuestState * 8 * 0 * 39
					stopMiracle * 0
					
					variable * 0 * 1
					tell * 205 * 5000
				end
				
				if variable * 2 * 0
				   variable * 3 * 2
				then
					newQuest * 8
					setQuestName * 8 * 38
					setQuestState * 8 * 2 * 41
					setQuestActif * 8 * 0
					stopMiracle * 0
					
					variable * 0 * 2
					tell * 206 * 5000
				end
				
				if variable * 2 * 0
				   variable * 3 * 0
				then
					speak_choice * 202 * 1
					speak_choice * 203 * 2	
					speak_choice * 204 * 3
					
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
					tell * 208 * 5000
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
		
		setQuestState * 8 * 1 * 40
		setQuestActif * 8 * 0
		
		if variable * 7 * 0
		then
			variable * 7 * 1
			tell * 207 * 5000
		end
	end
	
end 
