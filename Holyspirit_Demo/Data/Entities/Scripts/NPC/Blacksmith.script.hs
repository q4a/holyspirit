main 
	
	if no_speak
	then
		variable * 2 * 0 //AJOUTER CHOIX
		variable * 3 * 0 //MOMENT DE LA DISCUTION
		variable * 5 * 0
		variable * 6 * 0 //DERNIER CHOIX
		variable * 8 * 0
	end
	
	if talk
	then
		setState * 0 * 0
		variable * 8 * 1
		look_hero
		
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
				stop_speak
				variable * 2 * 0
				variable * 5 * 0
				craft
			end
			if speak_choice * 6
			then
				variable * 6 * 6
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

		
		if  variable * 5 * 0
		then
			if variable * 4 * 0
			then
				if player_class " Data/Miscs/Heroes/Concubine/Concubine.class.hs"
				then
					speak * 113
				end
				if player_class " Data/Miscs/Heroes/Crusader/Crusader.class.hs"
				then
					speak * 112
				end
				variable * 4 * 1
			else
				if player_class " Data/Miscs/Heroes/Concubine/Concubine.class.hs"
				then
					speak * 125
				end
				if player_class " Data/Miscs/Heroes/Crusader/Crusader.class.hs"
				then
					speak * 124
				end
			end
			
			variable * 5 * 1
		end
		
		if variable * 2 * 0
		   variable * 3 * 1
		then
			speak * 117
			variable * 3 * 0
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
		   variable * 3 * 3
		then
			if player_class " Data/Miscs/Heroes/Concubine/Concubine.class.hs"
			then
				speak * 130
				giftItem * 1
			else
				speak * 129
				giftItem * 0
			end
		
			variable * 3 * 0
			
			newQuest * 2 
			setQuestName * 2 * 13
			setQuestState * 2 * 0 * 14
		end
		
		if variable * 2 * 0
		   variable * 3 * 0
		then
			if variable * 6 * 1
			then else
				speak_choice * 116 * 1	
			end
			if variable * 6 * 2
			then else
				speak_choice * 118 * 2	
			end
			
			speak_choice * 121 * 3	
			
			if quest * 2 * -1
			then
				speak_choice * 128 * 6	
			else
				speak_choice * 122 * 4	
			end
			speak_choice * 123 * 5
			
			variable * 2 * 1
		end
	end
	
	if variable * 7 * day
	then
	else
		variable * 7 * day
		regenerate_inventory
	end
	
end 
