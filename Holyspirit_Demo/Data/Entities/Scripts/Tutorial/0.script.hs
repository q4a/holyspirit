
main 
	if distance * 2
	then
		if variable * 0 * 0
		then
			if no_speak
			then
				if player_class " Data/Miscs/Heroes/Concubine/Concubine.class.hs"
				then
					speak * 32 * 1
				else
					speak * 31 * 1
				end
				
				speak_choice * 43 * 1
			else
				if speak_choice * 1
				then
					variable * 0 * 1
					stop_speak
				end
			end
		end
	end
end 
