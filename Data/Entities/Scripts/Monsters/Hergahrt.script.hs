
main 
	if alive
	then
		if quest * 0 * 4
		then
			variable * 0 * 1
		end

		if quest * 0 * 5
		then
			if variable * 1 * 0
			then
				goto * 135 * 75
				teleport * 1	
				variable * 1 * 1
				uuseMiracle * 4
			else
				if position * 134 * 81
				then
					if variable * 1 * 1
					then
						stop_speak
						variable * 1 * 2
					end
					
					look_hero
					
					
				
					if no_speak
					then 
						speak_choice * 301 * 0
					end
					
					speak * 300
					
					if speak_choice * 0
					then
						setQuestState * 0 * 6 * 4
						stop_speak
						variable * 1 * 0
					end
				else
					goto * 134 * 81
				end
			end
		end

		if quest * 0 * 6
		then	
		
			if variable * 1 * 0
			then
				useMiracle * 5
				variable * 1 * 1
			end
		
			if no_speak
			then 
				speak_choice * 303 * 0
			end
			
			speak * 302
			
			if speak_choice * 0
			then
				setQuestState * 0 * 7 * 6
				variable * 1 * 0
				stop_speak
			end
		end
		
		if quest * 0 * 7
		then
			if variable * 1 * 0
			then
				variable * 1 * 1
				useMiracle * 4
			end
			
			if miracle * 4
			then
				goto * 1 * 1
				teleport * 1	
			end
		end
		
		
		if quest * 11 * 0
			variable * 1 * 1
			variable * 0 * -1

			if getState * 4 
				if see
				then
					useMiracle * 2
				else
					setState * 0
				end
			then
			else
				if see
				then
					variable * 0 * -1
					if distance * 2 
					then
						if rand * 25 
						then
							evasion
						else
							if rand * 75 
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
					if rand * 75
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
	else
		setQuestState * 11 * 1 * 68
	end
end 
