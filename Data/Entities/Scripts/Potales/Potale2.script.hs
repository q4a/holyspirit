main 
	if distance * 5
		variable * 0 * 0
	then
		variable * 0 * 1
		add_checkpoint * 58 * 45 * 8 " BurialGround.map.hs"
	end

	if talk
	then
		add_checkpoint * 58 * 45 * 8 " BurialGround.map.hs"
	
		if variable * 1 * 0 
		then
			speak * 1
			variable * 1 * 1
			speak_choice * 2 * 1
			speak_choice * 3 * 2
		end
		
		if speak_choice * 1
		then
			stop_speak
			teleportation_menu
			variable * 1 * 0
		end
		
		if speak_choice * 2
		then
			stop_speak
			trade * 1
			variable * 1 * 0
		end
		
	else
		variable * 1 * 0
		variable * 0 * 0
	end
end 