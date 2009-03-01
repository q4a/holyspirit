
main 
	variable * 0 * -1

	if see
	then
		variable * 0 * -1
		if distance * 2 
		then
			if rand * 75 
			then
				evasion
			else
				if rand * 10 
				then
					if numberInvocation * 0
					then
						variable * 0 * 1
					else
						variable * 0 * 0
					end
				else
					variable * 0 * 0
				end  
			end
		else
			if distance * 3 
			then
				evasion  
			else
				if distance * 6 
				then
					
						if numberInvocation * 0
						then
							variable * 0 * 1
						else
							variable * 0 * 0
						end
					
	
				else
					fight
				end
			end
		end
		
	else
		variable * 0 * -1
		randomDisplace  
	end
	
	if variable * 0 * 0
	then
		if rand * 25
		then
			useMiracle * 1
		else
			useMiracle * 0
		end
	end

	if variable * 0 * 1
	then
		useMiracle * 2
	end

end 
