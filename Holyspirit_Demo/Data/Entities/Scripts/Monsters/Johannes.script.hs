
main 
	if alive
	then
		if variable * 0 * 1
		then
			if variable * 1 * 0
			then
				goto * 8 * 153
				teleport * 1		
				tell * 213 * 5000
				variable * 1 * 1
				useMiracle * 0
			end
		
		
			if position * 8 * 153
			then 
				if variable * 1 * 1
				then
					if miracle * 0
					then
					else
						setState * 2 * 0
						variable * 1 * 2
					end
				end
				
				if variable * 1 * 2
					getState * 0
				then
					useMiracle * 0
					setState * 4 * 0
				end
				
				if variable * 1 * 2
					miracle * 0
				then
					goto * 11 * 150
					teleport * 1
					variable * 0 * 2
					variable * 1 * 0
					useMiracle * 0
				end
			end
		end
		
		if variable * 0 * 2
			distance * 3
		then
			if variable * 1 * 0
			then
				useMiracle * 0
				setState * 4 * 0		
				tell * 214 * 5000
				variable * 1 * 1
			end
			
			
			if variable * 1 * 1
				miracle * 0
			then
				goto * 10 * 139
				teleport * 1
				variable * 0 * 3
				useMiracle * 0
				variable * 1 * 0
			end
		end
		
		
		if variable * 0 * 3
			distance * 3
		then
			if variable * 1 * 0
			then
				useMiracle * 0
				setState * 4 * 0		
				tell * 215 * 5000
				variable * 1 * 1
			end
			
			
			if variable * 1 * 1
				miracle * 0
			then
				goto * 10 * 136
				teleport * 1
				variable * 0 * 4
				useMiracle * 0
				variable * 1 * 0
			end
		end
		
		if variable * 0 * 4
		then
		end
		
		if variable * 0 * 5
		then
			if variable * 1 * 0
			then
				useMiracle * 0
				setState * 4 * 0	
				variable * 1 * 1
			end
			
			
			if variable * 1 * 1
				miracle * 0
			then
				goto * 16 * 105
				teleport * 1
				variable * 0 * 3
				useMiracle * 0
				variable * 1 * 0
			end
		end
		
		
		if variable * 1 * 99
		then
		
		if variable * 0 * 2
		then
			if getState * 0 * 0
			then
				useMiracle * 0
				setState * 4
			end
			
			if miracle * 0
			then
				goto * 11 * 150
				teleport
				tell * 214 * 5000
			end
			
			if position * 11 * 150
			then
				variable * 0 * 3
			end
		end
		
		if variable * 0 * 3
			distance * 2
		then
				goto * 10 * 136
				teleport
			
				setState * 2 * 0
				
				variable * 0 * 4
		end
		
		if variable * 0 * 4
			distance * 4
		then
				tell * 215 * 5000
				variable * 0 * 5
		end
		end
	end
end 
