
main 

	
	
	if variable * 0 * 1
	then
		setFriendly * 0
		stop_speak
	
		if alive
		then
			if see
			then
				if distance * 4
				then
					if distance * 2
					then
						useMiracle * 0
					else
						fight
					end
				else
					useMiracle * 1
				end
			else
				randomDisplace
			end
		else
			if variable * 1 * 0
			then
				stopMiracle * 0
				stopMiracle * 1
				
				useMiracle * 2
				
				if miracle * 2
				then
				variable * 1 * 1
				end
			end
		end
	else
		if talk
		then
		
			if variable * 3 * 0
			then
				if variable * 4 * 0
				then
					if player_class " Data/Miscs/Heroes/Concubine/Concubine.class.hs"
					then
						speak * 256
					else
						speak * 255
					end
					
					speak_choice * 257 * 1
					
					if quest * 10 * 0
					then
						speak_choice * 262 * 2
					end
					
					speak_choice * 264 * 3
					
					if quest * 8 * 0
					then
						speak_choice * 268 * 4
					end
					
					variable * 4 * 1
				end
			
				if speak_choice * 1
				then	
					stop_speak
					variable * 4 * 0
					variable * 3 * 1
				end
			
				if speak_choice * 2
				then	
					stop_speak
					variable * 4 * 0
					variable * 3 * 2
				end
			
				if speak_choice * 3
				then	
					stop_speak
					variable * 4 * 0
					variable * 3 * 3
				end
			
				if speak_choice * 4
				then	
					stop_speak
					variable * 4 * 0
					variable * 3 * 4
				end
			end
			
			
			if variable * 3 * 1
			then
				if variable * 4 * 0
				then
					speak * 258
					
					speak_choice * 259 * 1
					
					if quest * 10 * 0
					then
						speak_choice * 262 * 2
					end
					
					speak_choice * 264 * 3
					
					if quest * 8 * 0
					then
						speak_choice * 268 * 4
					end
					
					variable * 4 * 1
				end
			
				if speak_choice * 1
				then	
					stop_speak
					variable * 4 * 0
					variable * 3 * 5
				end
			
				if speak_choice * 2
				then	
					stop_speak
					variable * 4 * 0
					variable * 3 * 2
				end
			
				if speak_choice * 3
				then	
					stop_speak
					variable * 4 * 0
					variable * 3 * 3
				end
			
				if speak_choice * 4
				then	
					stop_speak
					variable * 4 * 0
					variable * 3 * 4
				end
			end
			
			if variable * 3 * 2
			then
				if variable * 4 * 0
				then
					speak * 263
					
					speak_choice * 257 * 1
					
					speak_choice * 264 * 3
					
					if quest * 8 * 0
					then
						speak_choice * 268 * 4
					end
					
					variable * 4 * 1
				end
			
				if speak_choice * 1
				then	
					stop_speak
					variable * 4 * 0
					variable * 3 * 1
				end
			
				if speak_choice * 3
				then	
					stop_speak
					variable * 4 * 0
					variable * 3 * 3
				end
			
				if speak_choice * 4
				then	
					stop_speak
					variable * 4 * 0
					variable * 3 * 4
				end
			end
			
			if variable * 3 * 3
			then
				if variable * 4 * 0
				then
					
					speak * 265
					
					speak_choice * 257 * 1
					
					if quest * 10 * 0
					then
						speak_choice * 262 * 2
					end
					
					speak_choice * 266 * 3
					
					if quest * 8 * 0
					then
						speak_choice * 268 * 4
					end
					
					variable * 4 * 1
				end
			
				if speak_choice * 1
				then	
					stop_speak
					variable * 4 * 0
					variable * 3 * 1
				end
			
				if speak_choice * 2
				then	
					stop_speak
					variable * 4 * 0
					variable * 3 * 2
				end
			
				if speak_choice * 3
				then	
					stop_speak
					variable * 4 * 0
					variable * 3 * 6
				end
			
				if speak_choice * 4
				then	
					stop_speak
					variable * 4 * 0
					variable * 3 * 4
				end
			end
			
			if variable * 3 * 4
			then
				if variable * 4 * 0
				then
					speak * 269
					
					speak_choice * 257 * 1
					
					if quest * 10 * 0
					then
						speak_choice * 262 * 2
					end
					
					speak_choice * 264 * 3
					
					variable * 4 * 1
				end
			
				if speak_choice * 1
				then	
					stop_speak
					variable * 4 * 0
					variable * 3 * 1
				end
			
				if speak_choice * 2
				then	
					stop_speak
					variable * 4 * 0
					variable * 3 * 2
				end
			
				if speak_choice * 3
				then	
					stop_speak
					variable * 4 * 0
					variable * 3 * 3
				end
			end
			
			if variable * 3 * 5
			then
				if variable * 4 * 0
				then
					if player_class " Data/Miscs/Heroes/Concubine/Concubine.class.hs"
					then
						speak * 261
					else
						speak * 260
					end
					
					speak_choice * 270 * 1
		
					variable * 4 * 1
				end
			
				if speak_choice * 1
				then	
					stop_speak
					stop_talk
					variable * 0 * 1
				end
			
			end
			
			if variable * 3 * 6
			then
				if variable * 4 * 0
				then
					speak * 267
					
					speak_choice * 270 * 1
		
					variable * 4 * 1
				end
			
				if speak_choice * 1
				then	
					stop_speak
					stop_talk
					variable * 0 * 1
				end
			
			end
			
		end
	end
end 
