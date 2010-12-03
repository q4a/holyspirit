main 
	if talk
	then
		add_checkpoint * 38 * 37 * 4 " Abbey.map.hs"
	
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
		end
		
		if speak_choice * 2
		then
			stop_speak
			trade * 1
		end
		
	else
		variable * 1 * 0
		variable * 0 * 0
	end
end 