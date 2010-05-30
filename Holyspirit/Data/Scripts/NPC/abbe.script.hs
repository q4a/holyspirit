main 

	if  stop
		distance * 3
	then
			look_hero
	end

	if talk
	then
		if no_speak
		then
			variable * 2 * 0
			variable * 3 * 0
		end
	
		if variable * 4 * 0
		then
			if variable * 2 * 0
			then
				if variable * 5 * 0
				then
					if player_class " Data/Entities/Heroes/Concubine.class.hs"
					then
						speak * 59
					end
					if player_class " Data/Entities/Heroes/Crusader.class.hs"
					then
						speak * 58
					end
					speak_choice * 60 * 1
					speak_choice * 66 * 2
					
				end
				
				variable * 2 * 1
			end
			
			if speak_choice * 1
			then
				if variable * 5 * 2
				then
					stop_speak
					variable * 2 * 0
					variable * 5 * 3
					
					speak * 65
					speak_choice * 66 * 2
				end
				if variable * 5 * 1
				then
					stop_speak
					variable * 2 * 0
					variable * 5 * 2
					
					speak * 63
					speak_choice * 64 * 1
				end
				if variable * 5 * 0
				then
					stop_speak
					variable * 2 * 0
					variable * 5 * 1
					
					speak * 61
					speak_choice * 62 * 1
				end
			end
			
			if speak_choice * 2
			then
				stop_speak
				variable * 2 * 0
				variable * 4 * 1
			end
		else
			if speak_choice * 1
			then
				stop_speak
				heal
				if rand * 50
				then
						speak * 30
				else
					speak * 31
				end
			end
			
			if speak_choice * 2
			then
				stop_speak
				bless
			end
			
			if speak_choice * 3
			then
				stop_speak
				trade
			end
			
			if speak_choice * 4
			then
				stop_speak
				variable * 2 * 0
				variable * 4 * 0
				variable * 5 * 0
			end
			
			if speak_choice * 5
			then
				stop_speak
				stop_talk
			end
		end
		
		if variable * 2 * 0
		   variable * 4 * 1
		then
			if variable * 3 * 0 
			then
				if player_class " Data/Entities/Heroes/Concubine.class.hs"
				then
					speak * 71
				else
					speak * 53
				end
				
				variable * 3 * 1
				speak_choice * 67 * 4
				speak_choice * 54 * 1
				speak_choice * 33 * 3
				speak_choice * 55 * 2
				speak_choice * 70 * 5
			end
			variable * 2 * 1
		end
	else
		if stop
		   distance * 3
		then
		else
			if no_speak
			then
				variable * 5 * 0
				if variable * 0 * 75
				then
					randomDisplace
					variable * 0 * 0
				end
				incrementVariable * 0 * 1
			end
		end
	end
	
	if variable * 7 * day
	then
	else
		variable * 7 * day
		regenerate_inventory
	end
end 
