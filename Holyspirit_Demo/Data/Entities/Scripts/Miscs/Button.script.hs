main 
	if variable * 1 * 0
	then
		if talk
		then
			if variable * 0 * 1
			then	
				variable * 0 * 0
				stop_talk
			else
				variable * 0 * 1
				stop_talk
			end
		end
	end

	if variable * 0 * 0 
	then
		setState * 0
	else
		setState * 1
	end
end 