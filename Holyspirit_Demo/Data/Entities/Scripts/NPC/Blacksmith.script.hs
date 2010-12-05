main 
	
	if no_speak
	then
		variable * 2 * 0 //AJOUTER CHOIX
		variable * 3 * 0 //MOMENT DE LA DISCUTION
		variable * 5 * 0
	end
	
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
			if speak_choice * 2
			then
				variable * 3 * 2
				variable * 2 * 0
				stop_speak
			end
			if speak_choice * 3
			then
				stop_speak
				variable * 2 * 0
				variable * 5 * 0
				trade
			end
			if speak_choice * 4
			then
				stop_speak
				variable * 2 * 0
				variable * 5 * 0
				craft
			end
			if speak_choice * 5
			then
				stop_speak
				stop_talk
			end
		end

		
		if  variable * 5 * 0
		then
			if player_class " Data/Miscs/Heroes/Concubine/Concubine.class.hs"
			then
				speak * 103
			end
			if player_class " Data/Miscs/Heroes/Crusader/Crusader.class.hs"
			then
				speak * 102
			end
			
			variable * 5 * 1
		end
		
		if variable * 2 * 0
		   variable * 3 * 1
		then
			speak * 107
			variable * 3 * 0
		end
		
		if variable * 2 * 0
		   variable * 3 * 2
		then
		
			if player_class " Data/Miscs/Heroes/Concubine/Concubine.class.hs"
			then
				speak * 110
			else
				speak * 109
			end
			
			variable * 3 * 0
		end
		
		if variable * 2 * 0
		   variable * 3 * 0
		then
			speak_choice * 106 * 1		
			speak_choice * 108 * 2	
			speak_choice * 111 * 3	
			speak_choice * 112 * 4	
			speak_choice * 113 * 5
			
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
