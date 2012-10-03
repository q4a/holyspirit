
main


	if variable * 9 * 0 
	then
		if alive
		then
			if see
			then
				
				if variable * 2 * 0
				then
					if noPlayingSound * 4	
					   noPlayingSound * 5
					   noPlayingSound * 6
					   noPlayingSound * 7
					   noPlayingSound * 8
					   noPlayingSound * 9
					then
							if rand * 83
							then 
								playSound * 4
							else
								if rand * 80
								then 
									playSound * 5
								else
									if rand * 75
									then 
										playSound * 6
									else
										if rand *  67
										then 
											playSound * 7
										else
											if rand * 50 
											then 
												playSound * 8
											else
												playSound * 9
											end
										end
									end
								end
							end
					end
					variable * 2 * 1
				end
				
				if distance * 3
				then
					fight
				else
					if distance * 10
					then
						useMiracle * 0
						if getState * 0
						then
							setState * 5 * 0
						end
					else
						fight
					end
				end
				
				
			else
				variable * 2 * 0
				randomDisplace
			end
		end
	else
		setState * 4 * 0
		setActif * 0
	end
end
