
main

	if distance * 12
	then
		variable * 1 * 0
	else
		variable * 1 * 1
	end
	
	if variable * 0 * 0
	then
		variable * 1 * 1
		variable * 0 * 1
	end
	
	if variable * 1 * 1
	then
		if hour * 21 * 23 
		then
			setActif * 1
			setCollision * 1
			variable * 9 * 0 
		else
			if hour * 0 * 6 
			then
				setActif * 1
				setCollision * 1
				variable * 9 * 0 
			else
				setActif * 0
				setState * 4 * 0
				setCollision * 0
				variable * 9 * 1 
			end
		end
	end
	
	
	
	
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
				fight
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
