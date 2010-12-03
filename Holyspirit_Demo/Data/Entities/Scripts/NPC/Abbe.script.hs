main 

	if  stop
		distance * 3
	then
			look_hero
	end
	
	
	if no_speak
	then
		variable * 2 * 0 //AJOUTER CHOIX
		variable * 3 * 0 //MOMENT DE LA DISCUTION
		variable * 5 * 0
	end
	
	if talk
	then
		
		if variable * 3 * 0
		then
			if speak_choice * 1
			then
				variable * 3 * 1
				variable * 2 * 0
				stop_speak
			end
			if speak_choice * 2
			then
				variable * 3 * 4
				variable * 2 * 0
				stop_speak
			end
			if speak_choice * 3
			then
				stop_speak
				variable * 2 * 0
				variable * 5 * 0
				trade
			end
			if speak_choice * 4
			then
				stop_speak
				variable * 2 * 0
				variable * 5 * 0
				bless
			end
			if speak_choice * 5
			then
				stop_speak
				stop_talk
			end
		end
		
		if variable * 3 * 1
		then
			if speak_choice * 1
			then
				variable * 3 * 2
				variable * 2 * 0
				stop_speak
			end
		end
		
		if variable * 3 * 2
		then
			if speak_choice * 1
			then
				variable * 3 * 3
				variable * 2 * 0
				stop_speak
			end
		end
		
		if variable * 3 * 3
		then
			if speak_choice * 1
			then
				variable * 3 * 0
				variable * 2 * 0
				stop_speak
			end
		end
		
		if  variable * 5 * 0
		then
			if variable * 4 * 0 //PREMIERE FOIS
			then
				if player_class " Data/Miscs/Heroes/Concubine/Concubine.class.hs"
				then
					speak * 48
				end
				if player_class " Data/Miscs/Heroes/Crusader/Crusader.class.hs"
				then
					speak * 47
				end
				variable * 4 * 1
			else
				if variable * 3 * 0
				then
					if quest * 0 * -1
					then
						//TEXTE NORMAL A CHANGER
						if player_class " Data/Miscs/Heroes/Concubine/Concubine.class.hs"
						then
							speak * 48
						end
						if player_class " Data/Miscs/Heroes/Crusader/Crusader.class.hs"
						then
							speak * 47
						end
					else
						if quest * 0 * 0
						then
							//TEXTE NORMAL A CHANGER
							if player_class " Data/Miscs/Heroes/Concubine/Concubine.class.hs"
							then
								speak * 48
							end
							if player_class " Data/Miscs/Heroes/Crusader/Crusader.class.hs"
							then
								speak * 47
							end
						end
					
						if quest * 0 * 1
						then
							speak * 57
						end
					end
				end
			end
			variable * 5 * 1
		end
		
		if variable * 2 * 0
		   variable * 3 * 0
		then
			speak_choice * 51 * 1		
			speak_choice * 59 * 2	
			speak_choice * 61 * 3	
			speak_choice * 62 * 4	
			speak_choice * 63 * 5
			
			variable * 2 * 1
		end
		
		if variable * 2 * 0
		   variable * 3 * 1
		then
			speak * 52
			speak_choice * 53 * 1
			variable * 2 * 1
		end
		
		if variable * 2 * 0
		   variable * 3 * 2
		then
			speak * 54
			speak_choice * 55 * 1
			variable * 2 * 1
		end
		
		if variable * 2 * 0
		   variable * 3 * 3
		then
			speak * 56
			variable * 3 * 0
			
			newQuest * 0 
			setQuestName * 0 * 0
			setQuestState * 0 * 0 * 1
		end
		
		if variable * 2 * 0
		   variable * 3 * 4
		then
			speak * 60
			variable * 3 * 0
		end
	else
		if stop
		   distance * 3
		then
		else
			if no_speak
			then
				if variable * 0 * 75
				then
					randomDisplace
					variable * 0 * 0
				end
				incrementVariable * 0 * 1
			end
		end
	end
	
	if variable * 7 * day
	then
	else
		variable * 7 * day
		regenerate_inventory
	end
end 
