
main
	setActif * 0
	if variable * 9 * 0 
	then
		if alive
		then
			if see
			then
				if variable * 2 * 0 
				   distance * 4
				then
					useMiracle * 0 
					setState * 4
					variable * 2 * 1
				end

				if variable * 2 * 1
				then
					if getState  * 4 
					then
					else
						fight
					end
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
		setState * 5
	end
end
