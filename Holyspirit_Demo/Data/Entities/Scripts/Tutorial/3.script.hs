
main 
	if distance * 2
	then
		if variable * 1 * 0
		then
			if no_speak
			then
				speak * 36 * 1
				speak_choice * 43 * 1
			end
		end
		
		if 	speak_choice * 1
			variable * 1 * 0
		then
			variable * 1 * 1
			stop_speak
		end
		
		if variable * 1 * 1
		then
			if no_speak
			then
				speak * 37 * 1
				speak_choice * 43 * 1
			end
		end
		
		if speak_choice * 1
			variable * 1 * 1
		then
			variable * 1 * 2
			stop_speak
		end
		
		if variable * 1 * 2
		then
			if no_speak
			then
				speak * 38 * 1
				speak_choice * 43 * 1
			end
		end
		
		if speak_choice * 1
			variable * 1 * 2
		then
			variable * 1 * 3
			stop_speak
		end
	end
end 
