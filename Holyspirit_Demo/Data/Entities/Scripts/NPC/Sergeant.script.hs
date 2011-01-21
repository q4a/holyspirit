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
			if speak_choice * 4
			then
				variable * 6 * 4
				variable * 3 * 3
				variable * 2 * 0
				stop_speak
			end
			if speak_choice * 3
			then
				stop_speak
				stop_talk
			end
		end
		
		
		if  variable * 5 * 0
		then
			if player_class " Data/Miscs/Heroes/Concubine/Concubine.class.hs"
			then
				speak * 149
			end
			if player_class " Data/Miscs/Heroes/Crusader/Crusader.class.hs"
			then
				speak * 148
			end
			variable * 5 * 1
		end
		
		if variable * 2 * 0
		   variable * 3 * 1
		then
			speak * 151
			variable * 3 * 0
		end
		
		if variable * 2 * 0
		   variable * 3 * 2
		then
			speak * 153
			variable * 3 * 0
		end
		
		if variable * 2 * 0
		   variable * 3 * 3
		then
			speak * 156
			variable * 3 * 0
		end
		
		if variable * 2 * 0
		   variable * 3 * 0
		then
			if variable * 6 * 1
			then else
				speak_choice * 150 * 1	
			end
			if variable * 6 * 2
			then else
				speak_choice * 152 * 2	
			end
			if variable * 6 * 4
			then else
				speak_choice * 155 * 4	
			end
			speak_choice * 154 * 3
			
			variable * 2 * 1
		end
	end
	
end 
