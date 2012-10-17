
main
	if variable * 9 * 1
	then
		if alive
		then
			if quest * 7 * 4
			then
				dammages * 2000
			end
			
			if quest * 0 * 4
			then
				hero_goto * 136 * 84
				if position * 135 * 81
				then
					look_hero
					if no_speak
					then 
						speak_choice * 298 * 0
					end
					
					speak * 297
					
					
					if speak_choice * 0
				exist_item * 1 * 0 " Quest_relic1.item.hs"
				exist_item * 1 * 0 " Quest_relic2.item.hs"
				exist_item * 1 * 0 " Quest_relic3.item.hs"
				exist_item * 1 * 0 " Quest_relic4.item.hs"
					then
						setQuestState * 0 * 5 * 4
						stop_speak
					end
				else
					goto * 135 * 81
				end
			end
			
			if quest * 0 * 5
			then
				hero_goto * 136 * 84
				speak * 299
			end
					
			
			if quest * 0 * 3
				variable * 5 * 0
				variable * 6 * 0
			then
				if clicOver
				then
					variable * 3 * 1
					variable * 4 * 0
					stop_talk
				end
				
				if variable * 3 * 1
				then
					incrementVariable * 4 * 1
					if variable * 4 * 50
					then
						variable * 3 * 0
					end
					evasion
				else
					if distance * 10
					then
						if distance * 8 
						then
							if distance * 2
							then
							else
								followHero
							end
						else
							followHero
						end
					else
						followHero
						teleport
					end
				end	
			end
		end
	end
end
