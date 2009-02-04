
main 
	if see
	then
		if distance * 2
		then
			if rand * 75
			then
				evasion
			else
				useMiracle * 0  
			end
		else
			if distance * 3
			then
				evasion  
			else
				if distance * 6
				then
					if rand *15
					then
						useMiracle * 2
					else
						if rand * 10
						then
							useMiracle * 1
						else
							useMiracle * 0
						end 
					end
				else
					fight
				end
			end
		end
		
	else
		randomDisplace  
	end
	
end 
