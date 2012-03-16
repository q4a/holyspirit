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
		variable * 6 * 0 //DERNIER CHOIX
	end
	
	if talk
	then
		
		if variable * 3 * 0
		then
			if speak_choice * 1
			then
				if quest * 0 * -1
				then
					variable * 3 * 1
				end
				
				if quest * 0 * 1
				then
					variable * 3 * 4
				end
				
				if quest * 0 * 2
				exist_item * 1 * 0 " Quest_relic1.item.hs"
				exist_item * 1 * 0 " Quest_relic2.item.hs"
				exist_item * 1 * 0 " Quest_relic3.item.hs"
				exist_item * 1 * 0 " Quest_relic4.item.hs"
				then
					variable * 3 * 5
				end
				
				variable * 6 * 1
				variable * 2 * 0
				stop_speak
			end
			if speak_choice * 2
			then
				variable * 6 * 2
				variable * 3 * 6
				variable * 2 * 0
				stop_speak
			end
			if speak_choice * 3
			then
				variable * 6 * 3
				stop_speak
				variable * 2 * 0
				variable * 5 * 0
				trade
			end
			if speak_choice * 4
			then
				variable * 6 * 4
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
		
		if variable * 3 * 4
		then
			if speak_choice * 1
			then
				variable * 3 * 7
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
							speak * 65
						end
						if player_class " Data/Miscs/Heroes/Crusader/Crusader.class.hs"
						then
							speak * 64
						end
					else
						//TEXTE NORMAL A CHANGER
						if player_class " Data/Miscs/Heroes/Concubine/Concubine.class.hs"
						then
							speak * 65
						end
						if player_class " Data/Miscs/Heroes/Crusader/Crusader.class.hs"
						then
							speak * 64
						end
					end
				end
			end
			variable * 5 * 1
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
			stopMiracle * 0
		end
		
		if variable * 2 * 0
		   variable * 3 * 4
		then
			speak * 57
			speak_choice * 73 * 1
			variable * 2 * 1
		end
		
		if variable * 2 * 0
		   variable * 3 * 7
		then
			speak * 74
			variable * 3 * 0
			
			setQuestState * 0 * 2 * 3
			stopMiracle * 0
		end
		
		if variable * 2 * 0
		   variable * 3 * 5
		then
			speak * 68
			variable * 3 * 0
			
			setQuestState * 0 * 3 * 4
			stopMiracle * 0
		end
		
		if variable * 2 * 0
		   variable * 3 * 6
		then
			speak * 60
			variable * 3 * 0
		end
		
		if variable * 2 * 0
		   variable * 3 * 0
		then
			if quest * 0 * -1
			then
				speak_choice * 51 * 1
			end
			if quest * 0 * 1
			then
				speak_choice * 66 * 1
			end
			
			if quest * 0 * 2
				exist_item * 1 * 0 " Quest_relic1.item.hs"
				exist_item * 1 * 0 " Quest_relic2.item.hs"
				exist_item * 1 * 0 " Quest_relic3.item.hs"
				exist_item * 1 * 0 " Quest_relic4.item.hs"
			then
				speak_choice * 67 * 1
			end
			
			if variable * 6 * 2
			then else
				speak_choice * 59 * 2
			end
			speak_choice * 61 * 3	
			speak_choice * 62 * 4	
			speak_choice * 63 * 5
			
			variable * 2 * 1
		end
		
	else
		if stop
		   distance * 3
		then
		else
			if no_speak
			then
				if variable_bigger * 0 * 5
				then
					randomDisplace
					variable * 0 * 0
				end
				incrementVariable * 0 * time
			end
		end
	end
	
	if variable * 7 * day
	then
	else
		variable * 7 * day
		regenerate_inventory
	end
	
	if miracle * 0
	then
	else
		variable * 8 * 0
	end
	
	if quest * 0 * -1
		variable * 8 * 0
	then
		variable * 8 * 1
		useMiracle * 0
	end
	
	if quest * 0 * 1
		variable * 8 * 0
	then
		variable * 8 * 1
		useMiracle * 0
	end
	
	if quest * 0 * 2
		variable * 8 * 0
		exist_item * 1 * 0 " Quest_relic1.item.hs"
		exist_item * 1 * 0 " Quest_relic2.item.hs"
		exist_item * 1 * 0 " Quest_relic3.item.hs"
		exist_item * 1 * 0 " Quest_relic4.item.hs"
	then
		variable * 8 * 1
		useMiracle * 0
	end
end 
