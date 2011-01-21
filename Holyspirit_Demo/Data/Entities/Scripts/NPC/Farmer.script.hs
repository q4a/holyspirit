main 
	
	if no_speak
	then
		variable * 2 * 0 //AJOUTER CHOIX
		variable * 3 * 0 //MOMENT DE LA DISCUTION
		variable * 5 * 0
		variable * 6 * 0 //DERNIER CHOIX
		variable * 1 * 0
	end
	
	if talk
	then
		
		if variable * 3 * 0
		then
			if speak_choice * 1
			then
				variable * 6 * 1
				variable * 3 * 1
				variable * 2 * 0
				stop_speak
			end
			if speak_choice * 2
			then
				variable * 6 * 2
				variable * 3 * 2
				variable * 2 * 0
				
				stop_speak
			end
			if speak_choice * 4
			then
				variable * 6 * 4
				variable * 3 * 5
				variable * 2 * 0
				variable * 1 * 1
				
				stop_speak
			end
			if speak_choice * 3
			then
				variable * 6 * 3
				variable * 3 * 3
				variable * 2 * 0
				
				stop_speak
				stop_talk
			end
		end
		
		if variable * 3 * 2
		then
			if speak_choice * 1
			then
				variable * 6 * 1
				variable * 3 * 4
				variable * 2 * 0
				
				newQuest * 4 
				setQuestName * 4 * 21
				setQuestState * 4 * 0 * 22
				stopMiracle * 0
				
				stop_speak
			end
			if speak_choice * 2
			then
				variable * 6 * 2
				variable * 3 * 6
				variable * 2 * 0
				
				stop_speak
				stop_talk
			end
		end
		
		if variable * 3 * 4
		then
			if speak_choice * 1
			then
				variable * 6 * 3
				variable * 3 * 3
				variable * 2 * 0
				
				stop_speak
				stop_talk
			end
		end
		
		if  variable * 5 * 0
		then
			if variable * 3 * 0
			then
				if getState * 1
				then
					setState * 1 * 9
				end
				
				if quest * 4 * -1
				then
					if player_class " Data/Miscs/Heroes/Concubine/Concubine.class.hs"
					then
						speak * 81
					end
					if player_class " Data/Miscs/Heroes/Crusader/Crusader.class.hs"
					then
						speak * 80
					end
					variable * 1 * 1
				else
					if quest * 4 * 2
					then
						speak * 88
						variable * 1 * 1
					else
						speak * 168
					end
				end
			end
			variable * 5 * 1
		end
		
		if variable * 3 * 1
		then
			speak * 83
			variable * 3 * 0
		end
		
		if variable * 3 * 2
		then
			speak * 85
		end
		
		if variable * 3 * 4
		then
			speak * 90
		end
		
		if variable * 3 * 5
		then
			speak * 170
			variable * 3 * 0
		end
		
		if variable * 2 * 0
		   variable * 3 * 2
		then
			speak_choice * 86 * 1
			speak_choice * 87 * 2
			
			variable * 2 * 1
		end
		
		if variable * 2 * 0
		   variable * 3 * 4
		then
			speak_choice * 89 * 1
			
			variable * 2 * 1
		end
		
		if variable * 2 * 0
		   variable * 3 * 0
		then
		
			if variable * 6 * 1
			then  else
				speak_choice * 82 * 1
			end
			
			if quest * 4 * -1
			then
				speak_choice * 84 * 2
			end
			
			if variable * 1 * 0
			then
				speak_choice * 169 * 4
			end
			
			speak_choice * 89 * 3
			
			variable * 2 * 1
		end
	end
	
	if miracle * 0
	then
	else
		variable * 8 * 0
	end
	
	if quest * 4 * -1
		variable * 8 * 0
	then
		variable * 8 * 1
		useMiracle * 0
	end
	
	incrementVariable * 7 * time
	if variable_bigger * 7 * 13
	then
		variable * 7 * 0
		setState * 1 * 0
	end
end 
