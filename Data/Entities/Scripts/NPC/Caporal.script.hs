
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
			if speak_choice * 6
			then
				variable * 6 * 6
				variable * 3 * 6
				variable * 2 * 0
				stop_speak
			end
			if speak_choice * 7
			then
				variable * 6 * 7
				variable * 3 * 7
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
				variable * 3 * 3
				variable * 2 * 0
				stop_speak
			end
			if speak_choice * 4
			then
				variable * 6 * 4
				variable * 3 * 4
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
			speak * 275
			variable * 5 * 1
		end
		
		if variable * 2 * 0
		   variable * 3 * 1
		then
			speak * 277
			variable * 3 * 0
		end
		if variable * 2 * 0
		   variable * 3 * 6
		then
			speak * 279
			variable * 3 * 0
		end
		if variable * 2 * 0
		   variable * 3 * 7
		then
			speak * 281
			variable * 3 * 0
		end
		
		if variable * 2 * 0
		   variable * 3 * 2
		then
			speak * 283
			variable * 3 * 0
		end
		
		if variable * 2 * 0
		   variable * 3 * 3
		then
			speak * 285
			variable * 3 * 0
		end
		
		if variable * 2 * 0
		   variable * 3 * 4
		then
			if player_class " Data/Miscs/Heroes/Concubine/Concubine.class.hs"
			then
				speak * 288
			else
				speak * 287
			end
			variable * 3 * 0
		end
		
		if variable * 2 * 0
		   variable * 3 * 5
		then
			speak * 161
			setQuestState * 9 * 0 * 55
			setQuestActif * 9 * 0
			variable * 3 * 0
		end
		
		if variable * 2 * 0
		   variable * 3 * 0
		then
			
			if variable * 6 * 6
			then
				speak_choice * 280 * 7 
			else
				if variable * 6 * 1
				then
					speak_choice * 278 * 6	
				else
					speak_choice * 276 * 1	
				end
			end
			
			if variable * 6 * 2
			then else
				speak_choice * 282 * 2	
			end
			if variable * 6 * 3
			then else
				speak_choice * 284 * 3	
			end
			if variable * 6 * 4
			then else
				speak_choice * 286 * 4	
			end
			
			speak_choice * 154 * 5
			
			variable * 2 * 1
		end
	else
		if see
		then
			fight
		end
	end
end 
