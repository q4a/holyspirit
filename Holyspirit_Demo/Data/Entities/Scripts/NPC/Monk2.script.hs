main 

	
	if no_speak
	then
		variable * 2 * 0 //AJOUTER CHOIX
		variable * 3 * 0 //MOMENT DE LA DISCUTION
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
				stop_speak
				stop_talk
			end
		end
		
		if variable * 3 * 2
		then
			if speak_choice * 1
			then
				variable * 6 * 1
				variable * 3 * 3
				variable * 2 * 0
				stop_speak
			end
		end
		
		if variable * 3 * 3
		then
			if speak_choice * 1
			then
				variable * 6 * 1
				variable * 3 * 4
				variable * 2 * 0
				stop_speak
			end
		end
		
		
		if variable * 2 * 0
		   variable * 3 * 1
		then
			speak * 184
			variable * 3 * 0
		end
		
		if variable * 2 * 0
		   variable * 3 * 2
		then
			speak * 187
			speak_choice * 188 * 1			
			variable * 2 * 1
		end
		
		if variable * 2 * 0
		   variable * 3 * 3
		then
			speak * 189
			speak_choice * 190 * 1			
			variable * 2 * 1
		end
		
		if variable * 2 * 0
		   variable * 3 * 4
		then
			speak * 191
		    variable * 3 * 0
		end
		
		if variable * 2 * 0
		   variable * 3 * 0
		then
			speak * 182
		then
		
		
		if variable * 2 * 0
		   variable * 3 * 0
		then
			if variable * 6 * 1
			then else
				speak_choice * 183 * 1
			end
			
			if quest * 0 * 2
			then
				speak_choice * 186 * 2
			end
			speak_choice * 185 * 3
			
			variable * 2 * 1
		end
	end
	
end 
