main 
	if quest * 8 * -1
	then
		if distance * 5
		then
			tell * 225 * 5000
			if getState * 4 
			then
			else
				setState * 4
			end
		end
	else
		if quest * 8 * 0
			quest * 10 * 1
		then
			setQuestState * 8 * 1 * 47
		end
	end

	if talk
	then
		
		if quest * 8 * -1
		then
			if variable * 4 * 0
			then
				if variable * 3 * 0
				then
					speak * 226
					speak_choice * 227 * 1
					variable * 3 * 1
				end
			
				if speak_choice * 1
				then	
					stop_speak
					variable * 3 * 0
					variable * 4 * 1
				end
			end
			
			if variable * 4 * 1
			then
			
			tell * 0 * 500
				if variable * 3 * 0
				then
					speak * 228
					speak_choice * 229 * 1
					variable * 3 * 1
				end
			
				if speak_choice * 1
				then	
					stop_speak
					variable * 3 * 0
					variable * 4 * 2
				end
			end
			
			if variable * 4 * 2
			then
				if variable * 3 * 0
				then
					speak * 230
					speak_choice * 231 * 1
					variable * 3 * 1
				end
			
				if speak_choice * 1
				then	
					stop_speak
					variable * 3 * 0
					variable * 4 * 3
				end
			end
			
			if variable * 4 * 3
			then
				if variable * 3 * 0
				then
					speak * 232
					speak_choice * 233 * 1
					variable * 3 * 1
				end
			
				if speak_choice * 1
				then	
					stop_speak
					variable * 3 * 0
					variable * 4 * 4
				end
			end
			
			if variable * 4 * 4
			then
				if variable * 3 * 0
				then
					speak * 234
					speak_choice * 235 * 1
					variable * 3 * 1
				end
			
				if speak_choice * 1
				then	
					stop_speak
					variable * 3 * 0
					variable * 4 * 5
				end
			end
			
			
			if variable * 4 * 5
			then
				if variable * 3 * 0
				then
					speak * 236
					speak_choice * 237 * 1
					speak_choice * 238 * 2
					variable * 3 * 1
				end
			
				if speak_choice * 1
				then	
					newQuest * 8
					setQuestName * 8 * 44
					setQuestState * 8 * 0 * 45
					stop_speak
					stop_talk
				end
			
				if speak_choice * 2
				then	
					stop_speak
					stop_talk
					tell * 246 * 5000
				end
			end
		
		else
			if variable * 4 * 0
			then
				if variable * 3 * 0
				then
					speak * 239
					
					if quest * 8 * -1
					then
						if quest * 10 * 1
						then 
							setQuestState * 8 * 1 * 47
						end
					end
					
					if quest * 8 * 1
					then
						speak_choice * 241 * 1
					end
					
					if quest * 8 * 2
						exist_item * 3 * 0 " Craft_wood_plank.item.hs"
					then
						speak_choice * 247 * 3
					end
					
					speak_choice * 240 * 2
					
					
					variable * 3 * 1
				end
			
				if speak_choice * 1
				then	
					stop_speak
					variable * 3 * 0
					variable * 4 * 1
				end
			
				if speak_choice * 2
				then	
					stop_speak
					stop_talk
				end
				
				if speak_choice * 3
				then	
					stop_speak
					stop_talk
					if exist_item * 3 * 1 " Craft_wood_plank.item.hs"
					then
						tell * 248 * 5000
						setQuestState * 8 * 3 * 49
						setQuestActif * 8 * 0
						setActif * 0
						setState * 0 * 0
					end
				end
			end
			
			if variable * 4 * 1
			then
				if variable * 3 * 0
				then
					speak * 242
					
					speak_choice * 243 * 1
				
					variable * 3 * 1
				end
			
				if speak_choice * 1
				then	
					stop_speak
					variable * 3 * 0
					variable * 4 * 2
				end
			end
			
			if variable * 4 * 2
			then
				if variable * 3 * 0
				then
					speak * 244
					
					setQuestState * 8 * 2 * 48
				
					variable * 3 * 1
				end
			end
			
		end
	else
		variable * 3 * 0
		variable * 4 * 0
	end
end 
