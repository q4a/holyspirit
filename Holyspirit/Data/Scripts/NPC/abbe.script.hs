main 

	if  stop
		distance * 3
	then
			look_hero
	end

	if talk
	then
		if variable * 2 * 0
		then
			if variable * 3 * 0 
			then
				speak * 53
				variable * 3 * 1
				speak_choice * 54 * 1
				speak_choice * 33 * 3
				speak_choice * 55 * 2
			end
			variable * 2 * 1
		end
		
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
	else
		if stop
			distance * 3
		then
			
		else
			if no_speak
			then
				variable * 2 * 0
				if variable * 0 * 75
				then
					randomDisplace
					variable * 0 * 0
				end
				incrementVariable * 0 * 1
				
				variable * 3 * 0
			end
		end
	end
end 
