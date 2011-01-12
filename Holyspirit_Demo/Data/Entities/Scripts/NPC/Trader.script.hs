main 
	
	if no_speak
	then
		variable * 2 * 0 //AJOUTER CHOIX
		variable * 3 * 0 //MOMENT DE LA DISCUTION
		variable * 5 * 0
		variable * 6 * 0 //DERNIER CHOIX
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
			if speak_choice * 3
			then
				variable * 6 * 3
				stop_speak
				variable * 2 * 0
				variable * 5 * 0
				trade
			end
			if speak_choice * 4
			then
				variable * 6 * 4
				variable * 3 * 3
				variable * 2 * 0
				stop_speak
			end
			if speak_choice * 5
			then
				stop_speak
				stop_talk
			end
		end
		
		if variable * 3 * 4
		then
			if speak_choice * 1
			then
				variable * 3 * 4
				variable * 2 * 0
				stop_speak
			end
		end

		
		if  variable * 5 * 0
		then
			if player_class " Data/Miscs/Heroes/Concubine/Concubine.class.hs"
			then
				speak * 93
			end
			if player_class " Data/Miscs/Heroes/Crusader/Crusader.class.hs"
			then
				speak * 92
			end
			
			variable * 5 * 1
		end
		
		if variable * 2 * 0
		   variable * 3 * 1
		then
			speak * 97
			speak_choice * 98 * 1
			variable * 3 * 4
			
			variable * 2 * 1
		end
		
		if variable * 2 * 0
		   variable * 3 * 2
		then
			speak * 101
			variable * 3 * 0
		end
		
		if variable * 2 * 0
		   variable * 3 * 3
		then
			if quest * 1 * 1
			then
				speak * 104
				variable * 3 * 0
				stopMiracle * 0
			end
		end
		
		if variable * 2 * 0
		   variable * 3 * 4
		then
			speak * 99
			variable * 3 * 0
			
			newQuest * 1 
			setQuestName * 1 * 8
			setQuestState * 1 * 0 * 9
			stopMiracle * 0
		end
		
		
		if variable * 2 * 0
		   variable * 3 * 2
		then
		
			if player_class " Data/Miscs/Heroes/Concubine/Concubine.class.hs"
			then
				speak * 120
			else
				speak * 119
			end
			
			variable * 3 * 0
		end
		
		if variable * 2 * 0
		   variable * 3 * 0
		then
			if quest * 1 * 1
			then
				speak_choice * 122 * 4	
			end
			
			if quest * 1 * -1
			then
				speak_choice * 96 * 1
			end
			
			if variable * 6 * 2
			then else
				speak_choice * 100 * 2	
			end
			
			speak_choice * 102 * 3	
			
			speak_choice * 105 * 5
			
			variable * 2 * 1
		end
	end
	
	if variable * 7 * day
	then
	else
		variable * 7 * day
		regenerate_inventory
	end
	
	if miracle * 0
	then
	else
		variable * 8 * 0
	end
	
	
	if quest * 1 * -1
		variable * 8 * 0
	then
		variable * 8 * 1
		useMiracle * 0
	end
	
	if quest * 1 * 1
		variable * 8 * 0
	then
		variable * 8 * 1
		useMiracle * 0
	end

	if climate * 0 * 4 * 5
	then
		tell * 106 * 5000
	end
	
end 
