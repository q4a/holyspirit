
main
	if variable * 9 * 0 
	then
		if alive
		then
			if see
			then
				if getState * 4
				then
					setState * 5 * 0
				else
					if getState * 5
					then
						if getState * 5 * 15
						then
							setState * 1 * 0
							setSpeed * 60
						end
					else
						fight
					end
				end
			else
				setState * 4
				randomDisplace
			end
		end
	end
end
