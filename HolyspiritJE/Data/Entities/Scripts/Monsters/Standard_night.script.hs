
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
				fight
			else
				randomDisplace
			end
		end
	else
		setState * 4 * 0
		setActif * 0
	end
end
