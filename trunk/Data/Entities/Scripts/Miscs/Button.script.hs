main 
	if variable * 1 * 0
	then
		if interact
		then
			if variable * 0 * 1
			then	
				variable * 0 * 0
				stop_interact
			else
				variable * 0 * 1
				stop_interact
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