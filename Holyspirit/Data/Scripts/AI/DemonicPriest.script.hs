
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
					useMiracle * 0  
				else
					fight
				end
			end
		end

		
	else
		randomDisplace  
	end
	
end 
