
main 
	if variable * 1 * 0
	then
		setActif * 1

		variable * 0 * 0
		if talk
		then
			variable * 0 * 1
			
			if variable * 2 * 0 
			then
				speak * 311
				variable * 2 * 1
				speak_choice * 2 * 1
			end
			
			if speak_choice * 1
			then
				stop_speak
				teleportation_menu
				variable * 2 * 0
			end
		else
			variable * 2 * 0
		end
	else
		setActif * 0
	end
end 
