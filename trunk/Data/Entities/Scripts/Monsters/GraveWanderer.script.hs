
main 
	setActif * 0
	if variable * 9 * 0 
	then
		if alive
		then
			if see
			then
				if variable * 2 * 0 
				then
					setState * 5
					variable * 2 * 1
				end
			
				if variable * 2 * 1
				then
					if getState  * 5 
					then
					else
						if variable * 0 * 0
						then
							randomDisplace
							variable * 0 * 1
						else
							if distance * 7
							    visible
							then
								if rand * 50
								then
									useMiracle * 0
								else
									randomDisplace
								end
							else
								fight
							end
						end
					end
				end
			else
				if variable * 2 * 1
				then
					randomDisplace
				end
			end
		end

		if variable * 2 * 1
		then
			setActif * 1
		end
	end
	
	
	if variable * 2 * 0
	then
		setState * 4
	end
end 
