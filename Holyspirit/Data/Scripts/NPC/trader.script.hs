main 
	if talk
	then
		if variable * 0 * 0 
		then
			playSound * 0
			speak * 32
			variable * 0 * 1
			speak_choice * 33 * 1
			speak_choice * 34 * 2
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
		
	else
		variable * 0 * 0
	end
end 
