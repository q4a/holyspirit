main 
	if talk
	then
		if variable * 2 * 0
		then
			if speak_choice * 1
			then
				if variable * 3 * 2
				then
					stop_speak
					variable * 0 * 0
					variable * 2 * 1
				end
				if variable * 3 * 1
				then
					stop_speak
					speak * 69
					speak_choice * 56 * 1
					variable * 3 * 2
				end
			end
		else
			if speak_choice * 1
			then
				stop_speak
				trade
			end

			if speak_choice * 3
			then
				stop_speak
				craft
			end

			if speak_choice * 4
			then
				stop_speak
				variable * 0 * 0
				variable * 2 * 0
				variable * 3 * 0
			end

			if speak_choice * 5
			then
				stop_speak
				speak * 73
				speak_choice * 75 * 6
			end
			
			if speak_choice * 6
			then
				stop_speak
				variable * 0 * 0
				variable * 2 * 1
			end

			if speak_choice * 2
			then
				stop_talk
				stop_speak
			end
		end
		
		if variable * 0 * 0 
		then
			if variable * 4 * 0
			then
				playSound * 0
				variable * 4 * 1
			end
			
			if variable * 2 * 0
			then
				if variable * 3 * 0 
				then
					playSound * 0
					speak * 68
					variable * 0 * 1
					speak_choice * 56 * 1
					variable * 3 * 1
				end
			else
				if player_class " Data/Entities/Heroes/Concubine.class.hs"
				then
					speak * 57
				else
					speak * 32
				end
				variable * 0 * 1
				speak_choice * 67 * 4
				speak_choice * 74 * 5
				speak_choice * 33 * 1
				speak_choice * 41 * 3
				speak_choice * 70 * 2
			end
		end
	else
		variable * 0 * 0
		variable * 3 * 0
		variable * 4 * 0
	end
	
	incrementVariable * 8 * time
	if variable_bigger * 8 * 5
	then
		setState * 4
		variable * 8 * 0
	end
	
	if variable * 7 * day
	then
	else
		variable * 7 * day
		regenerate_inventory
	end
	
end 
