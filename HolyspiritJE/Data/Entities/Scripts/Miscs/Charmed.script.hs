
main
	if alive
	then
		if clicOver
		then
			variable * 7 * 1
			variable * 8 * 0
			stop_talk
		end
		
		if variable * 7 * 1
		then
			incrementVariable * 8 * 1
			if variable * 8 * 50
			then
				variable * 7 * 0
			end
			evasion
		else
			if distance * 10
			then
				if distance * 8 
				then
					if see
					then
					else
						if distance * 2
						then
							randomDisplace
						else
							followHero
						end
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
