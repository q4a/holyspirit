
main 
	if alive
	then
		variable * 1 * 1
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
					if rand * 25 
					then
						if numberInvocation * 0
						then
							variable * 0 * 1
						else
							variable * 0 * 2
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
				if rand * 35
				then
					if rand * 50
					then
						useMiracle * 3
					else
						useMiracle * 1
					end
				else
					useMiracle * 0
				end
			end
	
			if variable * 0 * 1
			then
				useMiracle * 2
				setState * 4
			end
	end
end 
