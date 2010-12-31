
main
	if variable * 9 * 1
	then
		if alive
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
