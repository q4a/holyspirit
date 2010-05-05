main 
	if talk
	then
		if variable * 0 * 0 
		then
			playSound * 0
			if player_class " Data/Entities/Heroes/Concubine.class.hs"
			then
				speak * 57
			else
				speak * 32
			end
			variable * 0 * 1
			speak_choice * 33 * 1
			speak_choice * 34 * 2
			speak_choice * 41 * 3
		end
		
		if speak_choice * 1
		then
			stop_speak
			trade
		end
		
		if speak_choice * 2
		then
			stop_speak
			speak * 35
		end

		if speak_choice * 3
		then
			stop_speak
			craft
		end
	else
		variable * 0 * 0
	end
end 
